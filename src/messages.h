/*
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	Copyright (C) 2012-2014 PIVA SOFTWARE (www.pivasoftware.com)
 *		Author: Mohamed Kallel <mohamed.kallel@pivasoftware.com>
 *		Author: Anis Ellouze <anis.ellouze@pivasoftware.com>
 *	Copyright (C) 2011 Luka Perkov <freecwmp@lukaperkov.net>
 */

#ifndef _EASYCWMP_MESSAGES_H__
#define _EASYCWMP_MESSAGES_H__

#define CWMP_INFORM_MESSAGE \
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"					\
"<soap_env:Envelope "															\
	"xmlns:soap_env=\"http://schemas.xmlsoap.org/soap/envelope/\" " 			\
	"xmlns:soap_enc=\"http://schemas.xmlsoap.org/soap/encoding/\" " 			\
	"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "							\
	"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "					\
	"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-2\">" 								\
		"<soap_env:Header>" 													\
		"<cwmp:ID soap_env:mustUnderstand=\"1\"/>"								\
	"</soap_env:Header>"														\
	"<soap_env:Body>"															\
	"<cwmp:Inform>" 															\
		"<DeviceId>"															\
			"<Manufacturer/>"													\
			"<OUI/>"															\
			"<ProductClass/>"													\
			"<SerialNumber/>"													\
		"</DeviceId>"															\
		"<Event soap_enc:arrayType=\"cwmp:EventStruct[0]\" />"					\
		"<MaxEnvelopes>1</MaxEnvelopes>"										\
		"<CurrentTime/>"														\
		"<RetryCount/>" 														\
		"<ParameterList soap_enc:arrayType=\"cwmp:ParameterValueStruct[0]\">"	\
		"</ParameterList>"														\
	"</cwmp:Inform>"															\
"</soap_env:Body>"																\
"</soap_env:Envelope>"


#define CWMP_RESPONSE_MESSAGE \
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"		\
"<soap_env:Envelope "												\
	"xmlns:soap_env=\"http://schemas.xmlsoap.org/soap/envelope/\" " \
	"xmlns:soap_enc=\"http://schemas.xmlsoap.org/soap/encoding/\" " \
	"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "				\
	"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "		\
	"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-2\">" 					\
	"<soap_env:Header>" 											\
		"<cwmp:ID soap_env:mustUnderstand=\"1\"/>"					\
	"</soap_env:Header>"											\
	"<soap_env:Body/>"												\
"</soap_env:Envelope>"

#define CWMP_GET_RPC_METHOD_MESSAGE \
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"		\
"<soap_env:Envelope "												\
	"xmlns:soap_env=\"http://schemas.xmlsoap.org/soap/envelope/\" " \
	"xmlns:soap_enc=\"http://schemas.xmlsoap.org/soap/encoding/\" " \
	"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "				\
	"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "		\
	"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-2\">" 					\
		"<soap_env:Header>" 										\
		"<cwmp:ID soap_env:mustUnderstand=\"1\"/>"					\
	"</soap_env:Header>"											\
	"<soap_env:Body>"												\
	"<cwmp:GetRPCMethods>"											\
	"</cwmp:GetRPCMethods>" 										\
"</soap_env:Body>"													\
"</soap_env:Envelope>"

#define CWMP_TRANSFER_COMPLETE_MESSAGE \
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"		\
"<soap_env:Envelope "												\
	"xmlns:soap_env=\"http://schemas.xmlsoap.org/soap/envelope/\" " \
	"xmlns:soap_enc=\"http://schemas.xmlsoap.org/soap/encoding/\" " \
	"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "				\
	"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "		\
	"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-2\">" 					\
		"<soap_env:Header>" 										\
		"<cwmp:ID soap_env:mustUnderstand=\"1\"/>"					\
	"</soap_env:Header>"											\
	"<soap_env:Body>"												\
	"<cwmp:TransferComplete>"										\
		"<CommandKey/>" 											\
		"<FaultStruct>" 											\
			"<FaultCode/>"											\
			"<FaultString/>"										\
		"</FaultStruct>"											\
		"<StartTime/>"												\
		"<CompleteTime/>"											\
	"</cwmp:TransferComplete>"										\
"</soap_env:Body>"													\
"</soap_env:Envelope>"

#define CWMP_GET_ALL_PARAMETER_NAME \
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"\
"<soap_env:Envelope "\
    "xmlns:soap_env=\"http://schemas.xmlsoap.org/soap/envelope/\" "\
    "xmlns:soap_enc=\"http://schemas.xmlsoap.org/soap/encoding/\" "\
    "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "\
    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "\
    "xmlns:cwmp=\"urn:dslforum-org:cwmp-1-2\">"\
    "<soap_env:Header>"\
        "<cwmp:ID soap_env:mustUnderstand=\"1\">%s</cwmp:ID>"\
    "</soap_env:Header>"\
    "<soap_env:Body>"\
        "<cwmp:GetParameterNamesResponse>"\
            "<ParameterList soap_enc:arrayType=\"cwmp:ParameterInfoStruct[41]\">"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.DeviceLog</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.HardwareVersion</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.Manufacturer</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.ManufacturerOUI</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.MemoryStatus.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.MemoryStatus.Free</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.MemoryStatus.Total</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.ProductClass</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.ProvisioningCode</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.SerialNumber</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.SoftwareVersion</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.SpecVersion</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.UpTime</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IP.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IP.Diagnostics.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IP.Diagnostics.IPPing.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IP.Interface.</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.AverageResponseTime</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.DataBlockSize</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.DiagnosticsState</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.FailureCount</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.Host</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.MaximumResponseTime</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.MinimumResponseTime</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.NumberOfRepetitions</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.SuccessCount</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.Timeout</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ConnectionRequestPassword</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ConnectionRequestURL</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ConnectionRequestUsername</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ParameterKey</Name>"\
                    "<Writable>0</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.Password</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.PeriodicInformEnable</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.PeriodicInformInterval</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.PeriodicInformTime</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.URL</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
                "<ParameterInfoStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.Username</Name>"\
                    "<Writable>1</Writable>"\
                "</ParameterInfoStruct>"\
            "</ParameterList>"\
        "</cwmp:GetParameterNamesResponse>"\
    "</soap_env:Body>"\
"</soap_env:Envelope>"


#define CWMP_GET_ALL_PARAMETER_VALUE \
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"\
"<soap_env:Envelope "\
"xmlns:soap_env=\"http://schemas.xmlsoap.org/soap/envelope/\" "\
"xmlns:soap_enc=\"http://schemas.xmlsoap.org/soap/encoding/\" "\
"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "\
"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "\
"xmlns:cwmp=\"urn:dslforum-org:cwmp-1-2\">"\
    "<soap_env:Header>"\
        "<cwmp:ID soap_env:mustUnderstand=\"1\">%s</cwmp:ID>"\
    "</soap_env:Header>"\
    "<soap_env:Body>"\
        "<cwmp:GetParameterValuesResponse>"\
            "<ParameterList soap_enc:arrayType=\"cwmp:ParameterValueStruct[32]\">"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.DeviceLog</Name>"\
                    "<Value xsi:type=\"xsd:string\"/>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.HardwareVersion</Name>"\
                    "<Value xsi:type=\"xsd:string\">1989</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.Manufacturer</Name>"\
                    "<Value xsi:type=\"xsd:string\">A</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.ManufacturerOUI</Name>"\
                    "<Value xsi:type=\"xsd:string\">B</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.MemoryStatus.Free</Name>"\
                    "<Value xsi:type=\"xsd:string\">541060</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.MemoryStatus.Total</Name>"\
                    "<Value xsi:type=\"xsd:string\">947748</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.ProductClass</Name>"\
                    "<Value xsi:type=\"xsd:string\">C</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.ProvisioningCode</Name>"\
                    "<Value xsi:type=\"xsd:string\"/>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.SerialNumber</Name>"\
                    "<Value xsi:type=\"xsd:string\">%s</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.SoftwareVersion</Name>"\
                    "<Value xsi:type=\"xsd:string\">0406</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.SpecVersion</Name>"\
                    "<Value xsi:type=\"xsd:string\">1.0</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.DeviceInfo.UpTime</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">12954</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.AverageResponseTime</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">0</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.DataBlockSize</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">1232</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.DiagnosticsState</Name>"\
                    "<Value xsi:type=\"xsd:string\">None</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.FailureCount</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">0</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.Host</Name>"\
                    "<Value xsi:type=\"xsd:string\"/>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.MaximumResponseTime</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">0</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.MinimumResponseTime</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">0</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.NumberOfRepetitions</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">3</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.SuccessCount</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">0</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.IPPingDiagnostics.Timeout</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">1232</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ConnectionRequestPassword</Name>"\
                    "<Value xsi:type=\"xsd:string\"/>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ConnectionRequestURL</Name>"\
                    "<Value xsi:type=\"xsd:string\">http://10.72.110.146:7547/</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ConnectionRequestUsername</Name>"\
                    "<Value xsi:type=\"xsd:string\">cpe</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.ParameterKey</Name>"\
                    "<Value xsi:type=\"xsd:string\"/>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.Password</Name>"\
                    "<Value xsi:type=\"xsd:string\"/>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.PeriodicInformEnable</Name>"\
                    "<Value xsi:type=\"xsd:boolean\">1</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.PeriodicInformInterval</Name>"\
                    "<Value xsi:type=\"xsd:unsignedInt\">3600</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.PeriodicInformTime</Name>"\
                    "<Value xsi:type=\"xsd:dateTime\">0000-00-00T00:00:00</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.URL</Name>"\
                    "<Value xsi:type=\"xsd:string\">http://10.72.110.56:7547</Value>"\
                "</ParameterValueStruct>"\
                "<ParameterValueStruct>"\
                    "<Name>InternetGatewayDevice.ManagementServer.Username</Name>"\
                    "<Value xsi:type=\"xsd:string\">easycwmp</Value>"\
                "</ParameterValueStruct>"\
            "</ParameterList>"\
        "</cwmp:GetParameterValuesResponse>"\
    "</soap_env:Body>"\
"</soap_env:Envelope>"

#endif
