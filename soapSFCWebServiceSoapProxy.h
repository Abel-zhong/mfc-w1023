/* soapSFCWebServiceSoapProxy.h
   Generated by gSOAP 2.8.58 for webservice.h

gSOAP XML Web services tools
Copyright (C) 2000-2017, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapSFCWebServiceSoapProxy_H
#define soapSFCWebServiceSoapProxy_H
#include "soapH.h"

    class SOAP_CMAC SFCWebServiceSoapProxy : public soap {
      public:
        /// Endpoint URL of service 'SFCWebServiceSoapProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in webservice.h, if any
        /// Construct a proxy with new managing context
        SFCWebServiceSoapProxy();
        /// Copy constructor
        SFCWebServiceSoapProxy(const SFCWebServiceSoapProxy& rhs);
        /// Construct proxy given a managing context
        SFCWebServiceSoapProxy(const struct soap&);
        /// Constructor taking an endpoint URL
        SFCWebServiceSoapProxy(const char *endpoint);
        /// Constructor taking input and output mode flags for the new managing context
        SFCWebServiceSoapProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        SFCWebServiceSoapProxy(const char *endpoint, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        SFCWebServiceSoapProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and managing context
        virtual ~SFCWebServiceSoapProxy();
        /// Initializer used by constructors
        virtual void SFCWebServiceSoapProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual SFCWebServiceSoapProxy *copy();
        /// Copy assignment
        SFCWebServiceSoapProxy& operator=(const SFCWebServiceSoapProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault string (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail as string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        /// Web service operation 'DoFunction' (returns SOAP_OK or error code)
        virtual int DoFunction(_ns1__DoFunction *ns1__DoFunction, _ns1__DoFunctionResponse &ns1__DoFunctionResponse)
        { return this->DoFunction(NULL, NULL, ns1__DoFunction, ns1__DoFunctionResponse); }
        virtual int DoFunction(const char *soap_endpoint, const char *soap_action, _ns1__DoFunction *ns1__DoFunction, _ns1__DoFunctionResponse &ns1__DoFunctionResponse);
        /// Web service operation 'UploadTestResult' (returns SOAP_OK or error code)
        virtual int UploadTestResult(_ns1__UploadTestResult *ns1__UploadTestResult, _ns1__UploadTestResultResponse &ns1__UploadTestResultResponse)
        { return this->UploadTestResult(NULL, NULL, ns1__UploadTestResult, ns1__UploadTestResultResponse); }
        virtual int UploadTestResult(const char *soap_endpoint, const char *soap_action, _ns1__UploadTestResult *ns1__UploadTestResult, _ns1__UploadTestResultResponse &ns1__UploadTestResultResponse);
        /// Web service operation 'CheckSSN' (returns SOAP_OK or error code)
        virtual int CheckSSN(_ns1__CheckSSN *ns1__CheckSSN, _ns1__CheckSSNResponse &ns1__CheckSSNResponse)
        { return this->CheckSSN(NULL, NULL, ns1__CheckSSN, ns1__CheckSSNResponse); }
        virtual int CheckSSN(const char *soap_endpoint, const char *soap_action, _ns1__CheckSSN *ns1__CheckSSN, _ns1__CheckSSNResponse &ns1__CheckSSNResponse);
        /// Web service operation 'CheckSSN_NEW' (returns SOAP_OK or error code)
        virtual int CheckSSN_USCORENEW(_ns1__CheckSSN_USCORENEW *ns1__CheckSSN_USCORENEW, _ns1__CheckSSN_USCORENEWResponse &ns1__CheckSSN_USCORENEWResponse)
        { return this->CheckSSN_USCORENEW(NULL, NULL, ns1__CheckSSN_USCORENEW, ns1__CheckSSN_USCORENEWResponse); }
        virtual int CheckSSN_USCORENEW(const char *soap_endpoint, const char *soap_action, _ns1__CheckSSN_USCORENEW *ns1__CheckSSN_USCORENEW, _ns1__CheckSSN_USCORENEWResponse &ns1__CheckSSN_USCORENEWResponse);
        /// Web service operation 'Binding' (returns SOAP_OK or error code)
        virtual int Binding(_ns1__Binding *ns1__Binding, _ns1__BindingResponse &ns1__BindingResponse)
        { return this->Binding(NULL, NULL, ns1__Binding, ns1__BindingResponse); }
        virtual int Binding(const char *soap_endpoint, const char *soap_action, _ns1__Binding *ns1__Binding, _ns1__BindingResponse &ns1__BindingResponse);
        /// Web service operation 'QueryBinding' (returns SOAP_OK or error code)
        virtual int QueryBinding(_ns1__QueryBinding *ns1__QueryBinding, _ns1__QueryBindingResponse &ns1__QueryBindingResponse)
        { return this->QueryBinding(NULL, NULL, ns1__QueryBinding, ns1__QueryBindingResponse); }
        virtual int QueryBinding(const char *soap_endpoint, const char *soap_action, _ns1__QueryBinding *ns1__QueryBinding, _ns1__QueryBindingResponse &ns1__QueryBindingResponse);
        /// Web service operation 'SfcTestResult_Upload' (returns SOAP_OK or error code)
        virtual int SfcTestResult_USCOREUpload(_ns1__SfcTestResult_USCOREUpload *ns1__SfcTestResult_USCOREUpload, _ns1__SfcTestResult_USCOREUploadResponse &ns1__SfcTestResult_USCOREUploadResponse)
        { return this->SfcTestResult_USCOREUpload(NULL, NULL, ns1__SfcTestResult_USCOREUpload, ns1__SfcTestResult_USCOREUploadResponse); }
        virtual int SfcTestResult_USCOREUpload(const char *soap_endpoint, const char *soap_action, _ns1__SfcTestResult_USCOREUpload *ns1__SfcTestResult_USCOREUpload, _ns1__SfcTestResult_USCOREUploadResponse &ns1__SfcTestResult_USCOREUploadResponse);
        /// Web service operation 'SaveSSN_NEW' (returns SOAP_OK or error code)
        virtual int SaveSSN_USCORENEW(_ns1__SaveSSN_USCORENEW *ns1__SaveSSN_USCORENEW, _ns1__SaveSSN_USCORENEWResponse &ns1__SaveSSN_USCORENEWResponse)
        { return this->SaveSSN_USCORENEW(NULL, NULL, ns1__SaveSSN_USCORENEW, ns1__SaveSSN_USCORENEWResponse); }
        virtual int SaveSSN_USCORENEW(const char *soap_endpoint, const char *soap_action, _ns1__SaveSSN_USCORENEW *ns1__SaveSSN_USCORENEW, _ns1__SaveSSN_USCORENEWResponse &ns1__SaveSSN_USCORENEWResponse);
        /// Web service operation 'test' (returns SOAP_OK or error code)
        virtual int test(_ns1__test *ns1__test, _ns1__testResponse &ns1__testResponse)
        { return this->test(NULL, NULL, ns1__test, ns1__testResponse); }
        virtual int test(const char *soap_endpoint, const char *soap_action, _ns1__test *ns1__test, _ns1__testResponse &ns1__testResponse);
        /// Web service operation 'GETMACID' (returns SOAP_OK or error code)
        virtual int GETMACID(_ns1__GETMACID *ns1__GETMACID, _ns1__GETMACIDResponse &ns1__GETMACIDResponse)
        { return this->GETMACID(NULL, NULL, ns1__GETMACID, ns1__GETMACIDResponse); }
        virtual int GETMACID(const char *soap_endpoint, const char *soap_action, _ns1__GETMACID *ns1__GETMACID, _ns1__GETMACIDResponse &ns1__GETMACIDResponse);
        /// Web service operation 'GetMacBySn' (returns SOAP_OK or error code)
        virtual int GetMacBySn(_ns1__GetMacBySn *ns1__GetMacBySn, _ns1__GetMacBySnResponse &ns1__GetMacBySnResponse)
        { return this->GetMacBySn(NULL, NULL, ns1__GetMacBySn, ns1__GetMacBySnResponse); }
        virtual int GetMacBySn(const char *soap_endpoint, const char *soap_action, _ns1__GetMacBySn *ns1__GetMacBySn, _ns1__GetMacBySnResponse &ns1__GetMacBySnResponse);
        /// Web service operation 'CartonWeight_Upload' (returns SOAP_OK or error code)
        virtual int CartonWeight_USCOREUpload(_ns1__CartonWeight_USCOREUpload *ns1__CartonWeight_USCOREUpload, _ns1__CartonWeight_USCOREUploadResponse &ns1__CartonWeight_USCOREUploadResponse)
        { return this->CartonWeight_USCOREUpload(NULL, NULL, ns1__CartonWeight_USCOREUpload, ns1__CartonWeight_USCOREUploadResponse); }
        virtual int CartonWeight_USCOREUpload(const char *soap_endpoint, const char *soap_action, _ns1__CartonWeight_USCOREUpload *ns1__CartonWeight_USCOREUpload, _ns1__CartonWeight_USCOREUploadResponse &ns1__CartonWeight_USCOREUploadResponse);
        /// Web service operation 'PCBData_Upload' (returns SOAP_OK or error code)
        virtual int PCBData_USCOREUpload(_ns1__PCBData_USCOREUpload *ns1__PCBData_USCOREUpload, _ns1__PCBData_USCOREUploadResponse &ns1__PCBData_USCOREUploadResponse)
        { return this->PCBData_USCOREUpload(NULL, NULL, ns1__PCBData_USCOREUpload, ns1__PCBData_USCOREUploadResponse); }
        virtual int PCBData_USCOREUpload(const char *soap_endpoint, const char *soap_action, _ns1__PCBData_USCOREUpload *ns1__PCBData_USCOREUpload, _ns1__PCBData_USCOREUploadResponse &ns1__PCBData_USCOREUploadResponse);
        /// Web service operation 'GetShaoluKey' (returns SOAP_OK or error code)
        virtual int GetShaoluKey(_ns1__GetShaoluKey *ns1__GetShaoluKey, _ns1__GetShaoluKeyResponse &ns1__GetShaoluKeyResponse)
        { return this->GetShaoluKey(NULL, NULL, ns1__GetShaoluKey, ns1__GetShaoluKeyResponse); }
        virtual int GetShaoluKey(const char *soap_endpoint, const char *soap_action, _ns1__GetShaoluKey *ns1__GetShaoluKey, _ns1__GetShaoluKeyResponse &ns1__GetShaoluKeyResponse);
        /// Web service operation 'ReleaseKey' (returns SOAP_OK or error code)
        virtual int ReleaseKey(_ns1__ReleaseKey *ns1__ReleaseKey, _ns1__ReleaseKeyResponse &ns1__ReleaseKeyResponse)
        { return this->ReleaseKey(NULL, NULL, ns1__ReleaseKey, ns1__ReleaseKeyResponse); }
        virtual int ReleaseKey(const char *soap_endpoint, const char *soap_action, _ns1__ReleaseKey *ns1__ReleaseKey, _ns1__ReleaseKeyResponse &ns1__ReleaseKeyResponse);
        /// Web service operation 'BindKey' (returns SOAP_OK or error code)
        virtual int BindKey(_ns1__BindKey *ns1__BindKey, _ns1__BindKeyResponse &ns1__BindKeyResponse)
        { return this->BindKey(NULL, NULL, ns1__BindKey, ns1__BindKeyResponse); }
        virtual int BindKey(const char *soap_endpoint, const char *soap_action, _ns1__BindKey *ns1__BindKey, _ns1__BindKeyResponse &ns1__BindKeyResponse);
        /// Web service operation 'GetSparrowCheckData' (returns SOAP_OK or error code)
        virtual int GetSparrowCheckData(_ns1__GetSparrowCheckData *ns1__GetSparrowCheckData, _ns1__GetSparrowCheckDataResponse &ns1__GetSparrowCheckDataResponse)
        { return this->GetSparrowCheckData(NULL, NULL, ns1__GetSparrowCheckData, ns1__GetSparrowCheckDataResponse); }
        virtual int GetSparrowCheckData(const char *soap_endpoint, const char *soap_action, _ns1__GetSparrowCheckData *ns1__GetSparrowCheckData, _ns1__GetSparrowCheckDataResponse &ns1__GetSparrowCheckDataResponse);
        /// Web service operation 'BlindKeyNum' (returns SOAP_OK or error code)
        virtual int BlindKeyNum(_ns1__BlindKeyNum *ns1__BlindKeyNum, _ns1__BlindKeyNumResponse &ns1__BlindKeyNumResponse)
        { return this->BlindKeyNum(NULL, NULL, ns1__BlindKeyNum, ns1__BlindKeyNumResponse); }
        virtual int BlindKeyNum(const char *soap_endpoint, const char *soap_action, _ns1__BlindKeyNum *ns1__BlindKeyNum, _ns1__BlindKeyNumResponse &ns1__BlindKeyNumResponse);
        /// Web service operation 'DoFunction' (returns SOAP_OK or error code)
        virtual int DoFunction_(_ns1__DoFunction *ns1__DoFunction, _ns1__DoFunctionResponse &ns1__DoFunctionResponse)
        { return this->DoFunction_(NULL, NULL, ns1__DoFunction, ns1__DoFunctionResponse); }
        virtual int DoFunction_(const char *soap_endpoint, const char *soap_action, _ns1__DoFunction *ns1__DoFunction, _ns1__DoFunctionResponse &ns1__DoFunctionResponse);
        /// Web service operation 'UploadTestResult' (returns SOAP_OK or error code)
        virtual int UploadTestResult_(_ns1__UploadTestResult *ns1__UploadTestResult, _ns1__UploadTestResultResponse &ns1__UploadTestResultResponse)
        { return this->UploadTestResult_(NULL, NULL, ns1__UploadTestResult, ns1__UploadTestResultResponse); }
        virtual int UploadTestResult_(const char *soap_endpoint, const char *soap_action, _ns1__UploadTestResult *ns1__UploadTestResult, _ns1__UploadTestResultResponse &ns1__UploadTestResultResponse);
        /// Web service operation 'CheckSSN' (returns SOAP_OK or error code)
        virtual int CheckSSN_(_ns1__CheckSSN *ns1__CheckSSN, _ns1__CheckSSNResponse &ns1__CheckSSNResponse)
        { return this->CheckSSN_(NULL, NULL, ns1__CheckSSN, ns1__CheckSSNResponse); }
        virtual int CheckSSN_(const char *soap_endpoint, const char *soap_action, _ns1__CheckSSN *ns1__CheckSSN, _ns1__CheckSSNResponse &ns1__CheckSSNResponse);
        /// Web service operation 'CheckSSN_NEW' (returns SOAP_OK or error code)
        virtual int CheckSSN_USCORENEW_(_ns1__CheckSSN_USCORENEW *ns1__CheckSSN_USCORENEW, _ns1__CheckSSN_USCORENEWResponse &ns1__CheckSSN_USCORENEWResponse)
        { return this->CheckSSN_USCORENEW_(NULL, NULL, ns1__CheckSSN_USCORENEW, ns1__CheckSSN_USCORENEWResponse); }
        virtual int CheckSSN_USCORENEW_(const char *soap_endpoint, const char *soap_action, _ns1__CheckSSN_USCORENEW *ns1__CheckSSN_USCORENEW, _ns1__CheckSSN_USCORENEWResponse &ns1__CheckSSN_USCORENEWResponse);
        /// Web service operation 'Binding' (returns SOAP_OK or error code)
        virtual int Binding_(_ns1__Binding *ns1__Binding, _ns1__BindingResponse &ns1__BindingResponse)
        { return this->Binding_(NULL, NULL, ns1__Binding, ns1__BindingResponse); }
        virtual int Binding_(const char *soap_endpoint, const char *soap_action, _ns1__Binding *ns1__Binding, _ns1__BindingResponse &ns1__BindingResponse);
        /// Web service operation 'QueryBinding' (returns SOAP_OK or error code)
        virtual int QueryBinding_(_ns1__QueryBinding *ns1__QueryBinding, _ns1__QueryBindingResponse &ns1__QueryBindingResponse)
        { return this->QueryBinding_(NULL, NULL, ns1__QueryBinding, ns1__QueryBindingResponse); }
        virtual int QueryBinding_(const char *soap_endpoint, const char *soap_action, _ns1__QueryBinding *ns1__QueryBinding, _ns1__QueryBindingResponse &ns1__QueryBindingResponse);
        /// Web service operation 'SfcTestResult_Upload' (returns SOAP_OK or error code)
        virtual int SfcTestResult_USCOREUpload_(_ns1__SfcTestResult_USCOREUpload *ns1__SfcTestResult_USCOREUpload, _ns1__SfcTestResult_USCOREUploadResponse &ns1__SfcTestResult_USCOREUploadResponse)
        { return this->SfcTestResult_USCOREUpload_(NULL, NULL, ns1__SfcTestResult_USCOREUpload, ns1__SfcTestResult_USCOREUploadResponse); }
        virtual int SfcTestResult_USCOREUpload_(const char *soap_endpoint, const char *soap_action, _ns1__SfcTestResult_USCOREUpload *ns1__SfcTestResult_USCOREUpload, _ns1__SfcTestResult_USCOREUploadResponse &ns1__SfcTestResult_USCOREUploadResponse);
        /// Web service operation 'SaveSSN_NEW' (returns SOAP_OK or error code)
        virtual int SaveSSN_USCORENEW_(_ns1__SaveSSN_USCORENEW *ns1__SaveSSN_USCORENEW, _ns1__SaveSSN_USCORENEWResponse &ns1__SaveSSN_USCORENEWResponse)
        { return this->SaveSSN_USCORENEW_(NULL, NULL, ns1__SaveSSN_USCORENEW, ns1__SaveSSN_USCORENEWResponse); }
        virtual int SaveSSN_USCORENEW_(const char *soap_endpoint, const char *soap_action, _ns1__SaveSSN_USCORENEW *ns1__SaveSSN_USCORENEW, _ns1__SaveSSN_USCORENEWResponse &ns1__SaveSSN_USCORENEWResponse);
        /// Web service operation 'test' (returns SOAP_OK or error code)
        virtual int test_(_ns1__test *ns1__test, _ns1__testResponse &ns1__testResponse)
        { return this->test_(NULL, NULL, ns1__test, ns1__testResponse); }
        virtual int test_(const char *soap_endpoint, const char *soap_action, _ns1__test *ns1__test, _ns1__testResponse &ns1__testResponse);
        /// Web service operation 'GETMACID' (returns SOAP_OK or error code)
        virtual int GETMACID_(_ns1__GETMACID *ns1__GETMACID, _ns1__GETMACIDResponse &ns1__GETMACIDResponse)
        { return this->GETMACID_(NULL, NULL, ns1__GETMACID, ns1__GETMACIDResponse); }
        virtual int GETMACID_(const char *soap_endpoint, const char *soap_action, _ns1__GETMACID *ns1__GETMACID, _ns1__GETMACIDResponse &ns1__GETMACIDResponse);
        /// Web service operation 'GetMacBySn' (returns SOAP_OK or error code)
        virtual int GetMacBySn_(_ns1__GetMacBySn *ns1__GetMacBySn, _ns1__GetMacBySnResponse &ns1__GetMacBySnResponse)
        { return this->GetMacBySn_(NULL, NULL, ns1__GetMacBySn, ns1__GetMacBySnResponse); }
        virtual int GetMacBySn_(const char *soap_endpoint, const char *soap_action, _ns1__GetMacBySn *ns1__GetMacBySn, _ns1__GetMacBySnResponse &ns1__GetMacBySnResponse);
        /// Web service operation 'CartonWeight_Upload' (returns SOAP_OK or error code)
        virtual int CartonWeight_USCOREUpload_(_ns1__CartonWeight_USCOREUpload *ns1__CartonWeight_USCOREUpload, _ns1__CartonWeight_USCOREUploadResponse &ns1__CartonWeight_USCOREUploadResponse)
        { return this->CartonWeight_USCOREUpload_(NULL, NULL, ns1__CartonWeight_USCOREUpload, ns1__CartonWeight_USCOREUploadResponse); }
        virtual int CartonWeight_USCOREUpload_(const char *soap_endpoint, const char *soap_action, _ns1__CartonWeight_USCOREUpload *ns1__CartonWeight_USCOREUpload, _ns1__CartonWeight_USCOREUploadResponse &ns1__CartonWeight_USCOREUploadResponse);
        /// Web service operation 'PCBData_Upload' (returns SOAP_OK or error code)
        virtual int PCBData_USCOREUpload_(_ns1__PCBData_USCOREUpload *ns1__PCBData_USCOREUpload, _ns1__PCBData_USCOREUploadResponse &ns1__PCBData_USCOREUploadResponse)
        { return this->PCBData_USCOREUpload_(NULL, NULL, ns1__PCBData_USCOREUpload, ns1__PCBData_USCOREUploadResponse); }
        virtual int PCBData_USCOREUpload_(const char *soap_endpoint, const char *soap_action, _ns1__PCBData_USCOREUpload *ns1__PCBData_USCOREUpload, _ns1__PCBData_USCOREUploadResponse &ns1__PCBData_USCOREUploadResponse);
        /// Web service operation 'GetShaoluKey' (returns SOAP_OK or error code)
        virtual int GetShaoluKey_(_ns1__GetShaoluKey *ns1__GetShaoluKey, _ns1__GetShaoluKeyResponse &ns1__GetShaoluKeyResponse)
        { return this->GetShaoluKey_(NULL, NULL, ns1__GetShaoluKey, ns1__GetShaoluKeyResponse); }
        virtual int GetShaoluKey_(const char *soap_endpoint, const char *soap_action, _ns1__GetShaoluKey *ns1__GetShaoluKey, _ns1__GetShaoluKeyResponse &ns1__GetShaoluKeyResponse);
        /// Web service operation 'ReleaseKey' (returns SOAP_OK or error code)
        virtual int ReleaseKey_(_ns1__ReleaseKey *ns1__ReleaseKey, _ns1__ReleaseKeyResponse &ns1__ReleaseKeyResponse)
        { return this->ReleaseKey_(NULL, NULL, ns1__ReleaseKey, ns1__ReleaseKeyResponse); }
        virtual int ReleaseKey_(const char *soap_endpoint, const char *soap_action, _ns1__ReleaseKey *ns1__ReleaseKey, _ns1__ReleaseKeyResponse &ns1__ReleaseKeyResponse);
        /// Web service operation 'BindKey' (returns SOAP_OK or error code)
        virtual int BindKey_(_ns1__BindKey *ns1__BindKey, _ns1__BindKeyResponse &ns1__BindKeyResponse)
        { return this->BindKey_(NULL, NULL, ns1__BindKey, ns1__BindKeyResponse); }
        virtual int BindKey_(const char *soap_endpoint, const char *soap_action, _ns1__BindKey *ns1__BindKey, _ns1__BindKeyResponse &ns1__BindKeyResponse);
        /// Web service operation 'GetSparrowCheckData' (returns SOAP_OK or error code)
        virtual int GetSparrowCheckData_(_ns1__GetSparrowCheckData *ns1__GetSparrowCheckData, _ns1__GetSparrowCheckDataResponse &ns1__GetSparrowCheckDataResponse)
        { return this->GetSparrowCheckData_(NULL, NULL, ns1__GetSparrowCheckData, ns1__GetSparrowCheckDataResponse); }
        virtual int GetSparrowCheckData_(const char *soap_endpoint, const char *soap_action, _ns1__GetSparrowCheckData *ns1__GetSparrowCheckData, _ns1__GetSparrowCheckDataResponse &ns1__GetSparrowCheckDataResponse);
        /// Web service operation 'BlindKeyNum' (returns SOAP_OK or error code)
        virtual int BlindKeyNum_(_ns1__BlindKeyNum *ns1__BlindKeyNum, _ns1__BlindKeyNumResponse &ns1__BlindKeyNumResponse)
        { return this->BlindKeyNum_(NULL, NULL, ns1__BlindKeyNum, ns1__BlindKeyNumResponse); }
        virtual int BlindKeyNum_(const char *soap_endpoint, const char *soap_action, _ns1__BlindKeyNum *ns1__BlindKeyNum, _ns1__BlindKeyNumResponse &ns1__BlindKeyNumResponse);
    };
#endif
