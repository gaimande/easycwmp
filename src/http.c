/*
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	Copyright (C) 2012-2015 PIVA SOFTWARE (www.pivasoftware.com)
 *		Author: Mohamed Kallel <mohamed.kallel@pivasoftware.com>
 *		Author: Anis Ellouze <anis.ellouze@pivasoftware.com>
 *	Copyright (C) 2011-2012 Luka Perkov <freecwmp@lukaperkov.net>
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <libubox/uloop.h>
#include <libubox/usock.h>
#include <curl/curl.h>

#include "http.h"
#include "config.h"
#include "cwmp.h"
#include "easycwmp.h"
#include "digestauth.h"
#include "log.h"
#include "messages.h"

#define MAX_WAIT_MSECS 30*1000 /* Maximum waiting time is 30 seconds */

struct list_head duts;
CURLM *multi_handle;

static struct http_client http_c;
static struct http_server http_s;

char *http_redirect_url = NULL;

int
http_client_init(void)
{
    int i;
    struct curl_client *nd, *pd;

    if (http_redirect_url) {
        if ((http_c.url = strdup(http_redirect_url)) == NULL)
            return -1;
    }
    else {
        if ((http_c.url = strdup(config->acs->url)) == NULL)
            return -1;
    }

    //DDF("+++ HTTP CLIENT CONFIGURATION +++\n");
    //DD("url: %s\n", http_c.url);
    if (config->acs->ssl_cert)
        DD("ssl_cert: %s\n", config->acs->ssl_cert);
    if (config->acs->ssl_cacert)
        DD("ssl_cacert: %s\n", config->acs->ssl_cacert);
    if (!config->acs->ssl_verify)
        DD("ssl_verify: SSL certificate validation disabled.\n");
    //DDF("--- HTTP CLIENT CONFIGURATION ---\n");

    DDF("\n\n----------------------------\n");

    list_for_each_entry_safe(nd, pd, &duts, list)
    {
        nd->curl = curl_easy_init();
        if (!nd->curl) return -1;
        
        curl_easy_setopt(nd->curl, CURLOPT_URL, http_c.url);
        curl_easy_setopt(nd->curl, CURLOPT_USERNAME, config->acs->username ? config->acs->username : "");
        curl_easy_setopt(nd->curl, CURLOPT_PASSWORD, config->acs->password ? config->acs->password : "");
        curl_easy_setopt(nd->curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC|CURLAUTH_DIGEST);
        curl_easy_setopt(nd->curl, CURLOPT_WRITEFUNCTION, http_get_response);
        curl_easy_setopt(nd->curl, CURLOPT_TIMEOUT, 30);
# ifdef DEVEL
        //curl_easy_setopt(d->curl, CURLOPT_VERBOSE, 1L);
# endif /* DEVEL */
        curl_easy_setopt(nd->curl, CURLOPT_COOKIEFILE, fc_cookies);
        curl_easy_setopt(nd->curl, CURLOPT_COOKIEJAR, fc_cookies);
        if (config->acs->ssl_cert)
            curl_easy_setopt(nd->curl, CURLOPT_SSLCERT, config->acs->ssl_cert);
        if (config->acs->ssl_cacert)
            curl_easy_setopt(nd->curl, CURLOPT_CAINFO, config->acs->ssl_cacert);
        if (!config->acs->ssl_verify)
            curl_easy_setopt(nd->curl, CURLOPT_SSL_VERIFYPEER, 0);

        DDF("\rCreating dut.. [%05d/%05d]", nd->idx, config->dut_cnt);
    }

    DDF("\nFinish creating duts\n");

    return 0;
}

void
http_client_exit(void)
{
        struct curl_client *nd, *pd;
        
    	FREE(http_c.url);
/*
    	if(curl) {
    	curl_easy_cleanup(curl);
    		curl = NULL;
    	}
    	curl_global_cleanup();
*/
        curl_multi_cleanup(multi_handle);
        list_for_each_entry_safe(nd, pd, &duts, list)
        {
                if (nd->curl)
                {
                        curl_easy_cleanup(nd->curl);
                        nd->curl = NULL;

                        FREE (nd->msg_in);
                        FREE (nd->msg_out);
                }
        }
        
    	if (access(fc_cookies, W_OK) == 0)
    		remove(fc_cookies);
}

static size_t
http_get_response(void *buffer, size_t size, size_t rxed, char **msg_in)
{
    char *c;

    if (asprintf(&c, "%s%.*s", *msg_in, size * rxed, buffer) == -1) {
        FREE(*msg_in);
        DDF("get response fail\n");
        return -1;
    }

    free(*msg_in);
    *msg_in = c;

    return size * rxed;
}

int8_t
http_send_message(char *msg_out, char **msg_in)
{
    /*
    	CURLcode res;
    	char error_buf[CURL_ERROR_SIZE] = "";

    	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg_out);
    	http_c.header_list = NULL;
    	http_c.header_list = curl_slist_append(http_c.header_list, "Accept:");
    	if (!http_c.header_list) return -1;
    	http_c.header_list = curl_slist_append(http_c.header_list, "User-Agent: easycwmp");
    	if (!http_c.header_list) return -1;
    	http_c.header_list = curl_slist_append(http_c.header_list, "Content-Type: text/xml; charset=\"utf-8\"");
    	if (!http_c.header_list) return -1;
    	if (config->acs->http100continue_disable) {
    		http_c.header_list = curl_slist_append(http_c.header_list, "Expect:");
    		if (!http_c.header_list) return -1;
    	}
    	if (msg_out) {
    		DDF("+++ SEND HTTP REQUEST +++\n");
    		DDF("%s", msg_out);
    		DDF("--- SEND HTTP REQUEST ---\n");
    		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(msg_out));
    		http_c.header_list = curl_slist_append(http_c.header_list, "SOAPAction;");
    		if (!http_c.header_list) return -1;
    	}
    	else {
    		DDF("+++ SEND EMPTY HTTP REQUEST +++\n");
    		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);
    	}
    	curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);
    	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);

    	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_c.header_list);

    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, msg_in);

    	*msg_in = (char *) calloc (1, sizeof(char));

    	res = curl_easy_perform(curl);

    	if (http_c.header_list) {
    		curl_slist_free_all(http_c.header_list);
    		http_c.header_list = NULL;
    	}

    	if (error_buf[0] != '\0')
    		log_message(NAME, L_NOTICE, "LibCurl Error: %s\n", error_buf);

    	if (!strlen(*msg_in)) {
    		FREE(*msg_in);
    	}

    	long httpCode = 0;
    	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    	if (httpCode == 302 || httpCode == 307) {
    		curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &http_redirect_url);
    		if ((http_redirect_url = strdup(http_redirect_url)) == NULL)
    			return -1;
    		http_client_exit();
    		if (http_client_init()) {
    			D("receiving http redirect: re-initializing http client failed\n");
    			FREE(http_redirect_url);
    			return -1;
    		}
    		FREE(http_redirect_url);
    		FREE(*msg_in);
    		int redirect = http_send_message(msg_out, msg_in);
    		return redirect;
    	}

    	if (res || (httpCode != 200 && httpCode != 204)) {
    		log_message(NAME, L_NOTICE, "sending http message failed\n");
    		return -1;
    	}

    	if (*msg_in) {
    		DDF("+++ RECEIVED HTTP RESPONSE +++\n");
    		DDF("%s", *msg_in);
    		DDF("--- RECEIVED HTTP RESPONSE ---\n");
    	} else {
    		DDF("+++ RECEIVED EMPTY HTTP RESPONSE +++\n");
    	}
    */
    return 0;
}

void
http_server_init(void)
{
    http_digest_init_nonce_priv_key();

    http_s.http_event.cb = http_new_client;

    http_s.http_event.fd = usock(USOCK_TCP | USOCK_SERVER, "0.0.0.0", config->local->port);
    uloop_fd_add(&http_s.http_event, ULOOP_READ | ULOOP_EDGE_TRIGGER);

    DDF("+++ HTTP SERVER CONFIGURATION +++\n");
    if (config->local->ip)
        DDF("ip: '%s'\n", config->local->ip);
    else
        DDF("NOT BOUND TO IP\n");
    DDF("port: '%s'\n", config->local->port);
    DDF("--- HTTP SERVER CONFIGURATION ---\n");

    log_message(NAME, L_NOTICE, "http server initialized\n");
}

static void
http_new_client(struct uloop_fd *ufd, unsigned events)
{
    int status;
    struct timeval t;

    t.tv_sec = 60;
    t.tv_usec = 0;

    for (;;) {
        int client = accept(ufd->fd, NULL, NULL);

        /* set one minute timeout */
        if (setsockopt(ufd->fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&t, sizeof t)) {
            DD("setsockopt() failed\n");
        }

        if (client == -1)
            break;

        struct uloop_process *uproc = calloc(1, sizeof(*uproc));
        if (!uproc || (uproc->pid = fork()) == -1) {
            FREE(uproc);
            close(client);
        }

        if (uproc->pid != 0) {
            /* parent */
            /* register an event handler for when the child terminates */
            uproc->cb = http_del_client;
            uloop_process_add(uproc);
            close(client);
        } else {
            /* child */
            FILE *fp;
            char buffer[BUFSIZ];
            char *auth_digest;
            int8_t auth_status = 0;

            fp = fdopen(client, "r+");

            DDF("+++ RECEIVED HTTP REQUEST +++\n");
            while (fgets(buffer, sizeof(buffer), fp)) {
                char *username = config->local->username;
                char *password = config->local->password;
                if (!username || !password) {
                    // if we dont have username or password configured proceed with connecting to ACS
                    auth_status = 1;
                }
                else if (auth_digest = strstr(buffer, "Authorization: Digest ")) {
                    if (http_digest_auth_check("GET", "/", auth_digest + strlen("Authorization: Digest "), REALM, username, password, 300) == MHD_YES)
                        auth_status = 1;
                    else {
                        auth_status = 0;
                        log_message(NAME, L_NOTICE, "Connection Request authorization failed\n");
                    }
                }
                if (buffer[0] == '\r' || buffer[0] == '\n') {
                    /* end of http request (empty line) */
                    goto http_end_child;
                }
            }
error_child:
            /* here we are because of an error, e.g. timeout */
            status = ETIMEDOUT|ENOMEM;
            goto done_child;

http_end_child:
            fflush(fp);
            if (auth_status) {
                status = 0;
                fputs("HTTP/1.1 200 OK\r\n", fp);
                fputs("Content-Length: 0\r\n", fp);
            } else {
                status = EACCES;
                fputs("HTTP/1.1 401 Unauthorized\r\n", fp);
                fputs("Content-Length: 0\r\n", fp);
                fputs("Connection: close\r\n", fp);
                http_digest_auth_fail_response(fp, "GET", "/", REALM, OPAQUE);
                fputs("\r\n", fp);
            }
            fputs("\r\n", fp);
            goto done_child;

done_child:
            fclose(fp);
            DDF("--- RECEIVED HTTP REQUEST ---\n");
            exit(status);
        }
    }
}

static void
http_del_client(struct uloop_process *uproc, int ret)
{
    FREE(uproc);

    /* child terminated ; check return code */
    if (WIFEXITED(ret) && WEXITSTATUS(ret) == 0) {
        DDF("+++ HTTP SERVER CONNECTION SUCCESS +++\n");
        log_message(NAME, L_NOTICE, "acs initiated connection\n");
        cwmp_connection_request(EVENT_CONNECTION_REQUEST);
    } else {
        DDF("+++ HTTP SERVER CONNECTION FAILED +++\n");
    }
}

static int duts_replace_serialnumber (struct curl_client **nd, const char *msg_out)
{
    char serial[32] = {0};
    char *serial_ptr = NULL;
    struct curl_client *dut = *nd;
    
    snprintf (serial, sizeof(serial), DUT_SERIALNUM, cwmp->deviceid.serial_number, dut->idx);
    
    dut->msg_out = strdup(msg_out);
    do
    {
        serial_ptr = strstr(dut->msg_out, cwmp->deviceid.serial_number);
        if (NULL == serial_ptr)
        {
            break;
        }
        
        memcpy (serial_ptr, serial, strlen(cwmp->deviceid.serial_number));
    } while (serial_ptr != NULL);

    return 0;
}

int duts_curl_setup (struct curl_client *nd)
{
    curl_easy_setopt(nd->curl, CURLOPT_POSTFIELDS, nd->msg_out);
    http_c.header_list = NULL;
    http_c.header_list = curl_slist_append(http_c.header_list, "Accept:");
    if (!http_c.header_list) return -1;
    http_c.header_list = curl_slist_append(http_c.header_list, "User-Agent: easycwmp");
    if (!http_c.header_list) return -1;
    http_c.header_list = curl_slist_append(http_c.header_list, "Content-Type: text/xml; charset=\"utf-8\"");
    if (!http_c.header_list) return -1;
    
    if (config->acs->http100continue_disable)
    {
        http_c.header_list = curl_slist_append(http_c.header_list, "Expect:");
        if (!http_c.header_list) return -1;
    }
    if (nd->msg_out)
    {
        curl_easy_setopt(nd->curl, CURLOPT_POSTFIELDSIZE, (long) strlen(nd->msg_out));
        http_c.header_list = curl_slist_append(http_c.header_list, "SOAPAction;");
        if (!http_c.header_list) return -1;
    }
    else {
        //DDF("+++ SEND EMPTY HTTP REQUEST +++\n");
        curl_easy_setopt(nd->curl, CURLOPT_POSTFIELDSIZE, 0);
    }
    curl_easy_setopt(nd->curl, CURLOPT_FAILONERROR, true);

    //curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buf);

    curl_easy_setopt(nd->curl, CURLOPT_HTTPHEADER, http_c.header_list);

    curl_easy_setopt(nd->curl, CURLOPT_WRITEDATA, &nd->msg_in);

    nd->msg_in = (char *) calloc (1, sizeof(char));

    return 0;
}

#if 0
int duts_inform_build (void)
{    
    struct curl_client *nd, *pd;
    char *msg_out;    

    if (xml_prepare_inform_message(&msg_out))
    {
        D("Inform xml message creating failed\n");
        return -1;
    }

    /* init a multi stack */
    multi_handle = curl_multi_init();

    DDF("\n\n----------------------------\n");

    list_for_each_entry_safe(nd, pd, &duts, list)
    {
        if (0 != duts_replace_serialnumber (&nd, msg_out))
        {
            DDF ("Error when building inform for %s\n");
            continue;
        }

        duts_curl_setup (nd);
        nd->inform_pending = 0;

        curl_multi_add_handle(multi_handle, nd->curl);

        DDF("\rBuilding inform for duts.. [%05d/%05d]", nd->idx, config->dut_cnt);
    }

    FREE(msg_out);

    DDF("\nFinish building inform\n");

    return 0;
}
#else
int duts_inform_build (void)
{
    struct list_head *p;
    struct event *event;
    struct curl_client *nd, *pd;
    int len;
    char serial[32] = {0};
    char *inform_msg;
    extern int force_periodic;
    
    /* init a multi stack */
    multi_handle = curl_multi_init();

    DDF("\n\n----------------------------\n");

    list_for_each_entry_safe(nd, pd, &duts, list)
    {
        memset (serial, 0x0, sizeof(serial));
        snprintf (serial, sizeof(serial), DUT_SERIALNUM, cwmp->deviceid.serial_number, nd->idx);

        inform_msg = NULL;
        if (1 == force_periodic)
        {
                inform_msg = CWMP_INFORM_PERIODIC_HARD;
        }
        else
        {
            list_for_each(p, &cwmp->events)
            {
                event = list_entry (p, struct event, list);
    
                if (event->code == EVENT_BOOTSTRAP || event->code == EVENT_BOOT)
                {
                    inform_msg = CWMP_INFORM_BOOT_HARD;
                }
                else if (event->code == EVENT_PERIODIC)
                {
                    inform_msg = CWMP_INFORM_PERIODIC_HARD;
                }
            }
        }
        
        if (NULL == inform_msg)
        {
            DDF ("[DUT %05d] Cannot build inform message.\n", nd->idx);
            return -1; 
        }

        len = sizeof(char) * (strlen(inform_msg) + 2 * strlen(serial) - 3);

        nd->msg_out = calloc(1, len);
        if (NULL == nd->msg_out)
        {
            DDF ("[DUT %05d] Not enough memory.\n", nd->idx);
            return -1;
        }

        snprintf (nd->msg_out, len, inform_msg, serial, serial);

        duts_curl_setup (nd);
        nd->inform_pending = 0;

        curl_multi_add_handle(multi_handle, nd->curl);

        DDF("\rBuilding inform for duts.. [%05d/%05d]", nd->idx, config->dut_cnt);
    }

    DDF("\nFinish building inform\n");

    return 0;    
}
#endif

int duts_inform_send(void)
{
    CURLMsg *msg = NULL; /* for picking up messages with the transfer status */
    char serial[32] = {0};
    struct timeval time_begin, time_end;
    double time_spent;
    
    struct curl_client *nd, *pd;
    char *msg_out = NULL;
    char *ptr_s, *ptr_e;
    int still_running; /* keep number of running handles */
    int msgs_left; /* how many messages are left */
    int fail_cnt = 0;
    int success = 0;
    int numfds=0;
    int res, len;

    /* we start some action by calling perform right away */
    DDF("\n\n----------------------------\n");
    DDF("Start to send inform message\n");

    gettimeofday(&time_begin, NULL);
    curl_multi_perform(multi_handle, &still_running);

    do
    {
        /* See how the transfers went */
        if (msg)
        {
            if(msg->msg == CURLMSG_DONE)
            {
                /* Find out which handle this message is about */
                list_for_each_entry_safe(nd, pd, &duts, list)
                {
                    if (msg->easy_handle == nd->curl)
                    {
                        curl_multi_remove_handle (multi_handle, nd->curl);
                        FREE(nd->msg_out);

                        if (!strlen(nd->msg_in))
                        {
                            FREE(nd->msg_in);
                        }

                        if (nd->msg_in)
                        {
                            if (0 == nd->inform_pending &&
                                0 == xml_parse_inform_response_message(nd->msg_in))
                            {
                                nd->inform_pending = 1;
                                DDF("[DUT %05d] Receive inform response message\n", nd->idx);
                            }
#if 0
                            else if (0 == xml_handle_message(nd->msg_in, &msg_out))
                            {
                                nd->request_pending = 1;
                                duts_replace_serialnumber (&nd, msg_out);
                                
                                FREE(msg_out);
                                DDF("[DUT %05d] Reply ACS request get\n", nd->idx);
                            }
#else
                            else if (NULL != strstr(nd->msg_in, "GetParameterNames"))
                            {
                                nd->request_pending = 1;
                                
                                /* Get cwmp:ID */
                                const char *cwmp_node = "<cwmp:ID soap-env:mustUnderstand=\"1\">";
                                
                                if (NULL != (ptr_s = strstr (nd->msg_in, cwmp_node)))
                                {
                                    ptr_s += strlen(cwmp_node);
                                    if (NULL != (ptr_e = strstr (ptr_s, "</cwmp:ID>")))
                                    {
                                        ptr_e[0] = '\0';
                                    }
                                }

                                if (NULL == ptr_e)
                                {
                                    fail_cnt++;
                                    DDF ("[DUT %05d] Cannot get cwmp:ID from ACS\n", nd->idx);
                                    break;
                                }

                                nd->cwmp_id = strdup(ptr_s);

                                len = sizeof(char) * (strlen(CWMP_GET_ALL_PARAMETER_NAME) + strlen(nd->cwmp_id) - 1);

                                nd->msg_out = calloc(1, len);
                                if (NULL == nd->msg_out)
                                {
                                    fail_cnt++;
                                    DDF ("[DUT %05d] Not enough memory.\n", nd->idx);
                                    break;
                                }
                                
                                snprintf (nd->msg_out, len, CWMP_GET_ALL_PARAMETER_NAME, nd->cwmp_id);

                                DDF("[DUT %05d] Reply ACS request get all params name\n", nd->idx);
                            }
                            else if (NULL != strstr(nd->msg_in, "GetParameterValues"))
                            {
                                nd->request_pending = 1;

                                memset (serial, 0x0, sizeof(serial));
                                snprintf (serial, sizeof(serial), DUT_SERIALNUM, cwmp->deviceid.serial_number, nd->idx);

                                len = sizeof(char) * (strlen(CWMP_GET_ALL_PARAMETER_VALUE) + strlen(nd->cwmp_id) + strlen(serial) - 3);

                                nd->msg_out = calloc(1, len);
                                if (NULL == nd->msg_out)
                                {
                                    fail_cnt++;
                                    DDF ("[DUT %05d] Not enough memory.\n", nd->idx);
                                    break;
                                }

                                snprintf (nd->msg_out, len, CWMP_GET_ALL_PARAMETER_VALUE, nd->cwmp_id, serial);
                                
                                DDF("[DUT %05d] Reply ACS request get all params value\n", nd->idx);
                            }
#endif                            
                            else
                            {
                                fail_cnt++;
                                DDF ("[DUT %05d] Sending http message failed!\n", nd->idx);
                                break;
                            }

                            //DDF("msg_out: %s\n", nd->msg_out);

                            FREE(nd->msg_in);
                            duts_curl_setup (nd);

                            curl_multi_add_handle(multi_handle, nd->curl);
                        }
                        else if (1 == nd->request_pending)
                        {
                            success++;

                            nd->request_pending = 0;
                            DDF("[DUT %05d] Finish TR-069 session\n", nd->idx);
                        }
                        else if (1 == nd->inform_pending)
                        {
                            success++;
                            nd->inform_pending = 0;
                            DDF("[DUT %05d] Finish TR-069 periodically inform session\n", nd->idx);
                        }
                        else
                        {
                            fail_cnt++;
                            DDF ("[DUT %05d] TR-069 session is failed\n", nd->idx);
                        }

                        break;
                    }
                }
            }
        }

        res = curl_multi_wait(multi_handle, NULL, 0, MAX_WAIT_MSECS, &numfds);
        if(res != CURLM_OK)
        {
            DDF("error: curl_multi_wait() returned %d\n", res);
            return EXIT_FAILURE;
        }

#if 0
        if(!numfds)
        {
            fprintf(stderr, "error: curl_multi_wait() numfds=%d\n", numfds);
            return EXIT_FAILURE;
        }
#endif

        curl_multi_perform(multi_handle, &still_running);

    } while((msg = curl_multi_info_read(multi_handle, &msgs_left)) || still_running);

    gettimeofday(&time_end, NULL);
    time_spent = (double) (time_end.tv_usec - time_begin.tv_usec) / 1000000 +
                 (double) (time_end.tv_sec - time_begin.tv_sec);
    fprintf(stderr, "\nFinished recieving ACS respone. Success: %d/%d. Failed: %d/%d. Elapsed: %f seconds.\n", success, config->dut_cnt, fail_cnt, config->dut_cnt, time_spent);

    return 0;
}
