
#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <modbus.h>
//#include <modbus-rtu.h>
//#include <modbus-tcp.h>
//#include <modbus-version.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> 
#include <sys/socket.h>

#include <iostream>

using namespace v8;
using namespace node;
using namespace std;

// finger to the sky
#define REPORT_LEN 0xFF

// modbus_t *modbus_new_rtu(const char *device, int baud, char parity, int data_bit, int stop_bit);
// External new_rtu(String, Integer, String, Integer, Integer);
//Handle<Value> js_new_rtu(const Arguments& args) {
Handle<Value> NewRtu(const Arguments& args) {
	String::Utf8Value device(args[0]);
	int baud = Local<Integer>::Cast(args[1])->Int32Value();
	String::Utf8Value parity_str(args[2]);
	int data_bit = Local<Integer>::Cast(args[3])->Int32Value();
	int stop_bit = Local<Integer>::Cast(args[4])->Int32Value();

	char parity = (*parity_str)[0];

	modbus_t *ctx = static_cast<modbus_t *>(modbus_new_rtu(*device, baud, parity, data_bit, stop_bit));

	if (ctx == NULL) return Null();

	return External::New(ctx);
}

// int modbus_rtu_get_serial_mode(modbus_t *ctx);
// Integer rtu_get_serial_mode(External);
Handle<Value> RtuGetSerialMode(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	int ret = modbus_rtu_get_serial_mode(ctx);
	
	return Integer::New(ret);
}

// int modbus_rtu_set_serial_mode(modbus_t *ctx, int mode);
// Integer rtu_set_serial_mode(External, Integer);
Handle<Value> RtuSetSerialMode(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int mode = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_rtu_set_serial_mode(ctx, mode);
	
	return Integer::New(ret);
}

/*
// int modbus_rtu_get_rts(modbus_t *ctx);
Handle<Value> js_rtu_get_rts(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Unwrap(args[0]));
	
	int ret = modbus_rtu_get_rts(ctx);
	
	return Integer::New(ret);
}

// int modbus_rtu_set_rts(modbus_t *ctx, int mode)
Handle<Value> js_rtu_set_rts(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Unwrap(args[0]));
	int mode = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_rtu_set_rts(ctx, mode);
	
	return Integer::New(ret);
}
*/

// modbus_t *modbus_new_tcp(const char *ip, int port);
// External new_tcp(String, Integer);
Handle<Value> NewTcp(const Arguments& args) {
	String::Utf8Value ip(args[0]);
	int port = Local<Integer>::Cast(args[1])->Int32Value();
	
	modbus_t *ctx = modbus_new_tcp(*ip, port);
	
	if (ctx == NULL) return Null();
	
	return External::New(ctx);
}

// modbus_t *modbus_new_tcp_pi(const char *node, const char *service);
// External new_tcp_pi(String, String);
Handle<Value> NewTcpPi(const Arguments& args) {
	String::Utf8Value node(args[0]);
	String::Utf8Value service(args[1]);
	
	modbus_t *ctx = modbus_new_tcp_pi(*node, *service);
	
	if (ctx == NULL) return Null();
	
	return External::New(ctx);
}

// void modbus_free(modbus_t *ctx);
// Undefined free(External);
Handle<Value> Free(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	modbus_free(ctx);
	
	return Undefined();
}

// void modbus_get_byte_timeout(modbus_t *ctx, struct timeval *timeout);
// Undefined get_byte_timeout(External, Object);
Handle<Value> GetByteTimeout(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Object> timeout_obj = Local<Object>::Cast(args[1]);
	
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	modbus_get_byte_timeout(ctx, &timeout);
	
	timeout_obj->Set(String::New("tv_sec"), Uint32::New(timeout.tv_sec));
	timeout_obj->Set(String::New("tv_usec"), Uint32::New(timeout.tv_usec));
	
	return Undefined();
}

// void modbus_set_byte_timeout(modbus_t *ctx, struct timeval *timeout);
// Undefined set_byte_timeout(External, Object);
Handle<Value> SetByteTimeout(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Object> timeout_obj = Local<Object>::Cast(args[1]);
	
	struct timeval timeout;
	timeout.tv_sec = timeout_obj->Get(String::New("tv_sec"))->Uint32Value();
	timeout.tv_usec = timeout_obj->Get(String::New("tv_usec"))->Uint32Value();
	modbus_get_byte_timeout(ctx, &timeout);
	
	return Undefined();
}

// void modbus_set_debug(modbus_t *ctx, int boolean);
// Undefined set_debug(External, Integer);
Handle<Value> SetDebug(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int boolean = Local<Integer>::Cast(args[1])->Int32Value();
	
	modbus_set_debug(ctx, boolean);
	
	return Undefined();
}

// int modbus_set_error_recovery(modbus_t *ctx, modbus_error_recovery_mode error_recovery);
// Integer set_error_recovery(External, Integer);
Handle<Value> SetErrorRecovery(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int error_recovery = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_set_error_recovery(ctx, static_cast<modbus_error_recovery_mode>(error_recovery));
	
	return Integer::New(ret);
}

// int modbus_get_header_length(modbus_t *ctx);
// Integer get_header_length(External);
Handle<Value> GetHeaderLength(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	int ret = modbus_get_header_length(ctx);
	
	return Integer::New(ret);
}

// void modbus_get_response_timeout(modbus_t *ctx, struct timeval *timeout);
// Undefined get_response_timeout(External, Object);
Handle<Value> GetResponseTimeout(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Object> timeout_obj = Local<Object>::Cast(args[1]);
	
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	modbus_get_response_timeout(ctx, &timeout);
	
	timeout_obj->Set(String::New("tv_sec"), Uint32::New(timeout.tv_sec));
	timeout_obj->Set(String::New("tv_usec"), Uint32::New(timeout.tv_usec));
	
	return Undefined();
}

// void modbus_set_response_timeout(modbus_t *ctx, struct timeval *timeout);
// Undefined set_response_timeout(External, Object);
Handle<Value> SetResponseTimeout(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Object> timeout_obj = Local<Object>::Cast(args[1]);
	
	struct timeval timeout;
	timeout.tv_sec = timeout_obj->Get(String::New("tv_sec"))->Uint32Value();
	timeout.tv_usec = timeout_obj->Get(String::New("tv_usec"))->Uint32Value();
	modbus_set_response_timeout(ctx, &timeout);
	
	return Undefined();
}

// int modbus_set_slave(modbus_t *ctx, int slave);
// Integer set_slave(External, Integer);
Handle<Value> SetSlave(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int slave = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_set_slave(ctx, slave);
	
	return Integer::New(ret);
}

// void modbus_set_socket(modbus_t *ctx, int socket);
// Undefined set_socket(External, Integer);
Handle<Value> SetSocket(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int socket = Local<Integer>::Cast(args[1])->Int32Value();
	
	modbus_set_socket(ctx, socket);
	
	return Undefined();
}

// int modbus_get_socket(modbus_t *ctx);
// Integer get_socket(External);
Handle<Value> GetSocket(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	int ret = modbus_get_socket(ctx);
	
	return Integer::New(ret);
}

// in js module
// void modbus_set_bits_from_byte(uint8_t *dest, int index, const uint8_t value);
// void modbus_set_bits_from_bytes(uint8_t *dest, int index, unsigned int nb_bits, const uint8_t *tab_byte);
// uint8_t modbus_get_byte_from_bits(const uint8_t *src, int index, unsigned int nb_bits);
// float modbus_get_float(const uint16_t *src);
// void modbus_set_float(float f, uint16_t *dest);

// int modbus_connect(modbus_t *ctx);
// Integer connect(External);
Handle<Value> Connect(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	int ret = modbus_connect(ctx);
	
	return Integer::New(ret);
}

// void modbus_close(modbus_t *ctx);
// Undefined close(External);
Handle<Value> Close(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	modbus_close(ctx);
	
	return Undefined();
}

// int modbus_flush(modbus_t *ctx);
// Integer flush(External);
Handle<Value> Flush(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	int ret = modbus_flush(ctx);
	
	return Integer::New(ret);
}

// int modbus_read_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest);
// Integer read_bits(External, Integer, Integer, Array);
Handle<Value> ReadBits(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> dest_arr = Local<Array>::Cast(args[3]);
	
	uint8_t dest[nb];
	int ret = modbus_read_bits(ctx, addr, nb, dest);
	
	for (int i = 0; i < nb; i++) dest_arr->Set(i, Number::New(dest[i]));
	
	return Integer::New(ret);
}

// int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest);
// Integer read_input_bits(External, Integer, Integer, Array);
Handle<Value> ReadInputBits(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> dest_arr = Local<Array>::Cast(args[3]);
	
	uint8_t dest[nb];
	int ret = modbus_read_input_bits(ctx, addr, nb, dest);
	
	for (int i = 0; i < nb; i++) dest_arr->Set(i, Number::New(dest[i]));
	
	return Integer::New(ret);
}

// int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);
// Integer read_registers(External, Integer, Integer, Array);
Handle<Value> ReadRegisters(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> dest_arr = Local<Array>::Cast(args[3]);
	
	uint16_t dest[nb];
	int ret = modbus_read_registers(ctx, addr, nb, dest);
	
	for (int i = 0; i < nb; i++) dest_arr->Set(i, Number::New(dest[i]));
	
	return Integer::New(ret);
}

// int modbus_read_input_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);
// Integer read_input_registers(External, Integer, Integer, Array);
Handle<Value> ReadInputRegisters(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> dest_arr = Local<Array>::Cast(args[3]);
	
	uint16_t dest[nb];
	int ret = modbus_read_input_registers(ctx, addr, nb, dest);
	
	for (int i = 0; i < nb; i++) dest_arr->Set(i, Number::New(dest[i]));
	
	return Integer::New(ret);
}

// int modbus_report_slave_id(modbus_t *ctx, uint8_t *dest);
// Integer report_slave_id(External, Array);
Handle<Value> ReportSlaveId(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Array> dest_obj = Local<Array>::Cast(args[1]);
	
	uint8_t dest[REPORT_LEN];
    memset(dest, 0, REPORT_LEN * sizeof(uint8_t));
	
	int ret = modbus_report_slave_id(ctx, dest);
	
	if (ret > 0) dest_obj->Set(0, Integer::New(dest[0])); // Slave ID
	if (ret > 1) dest_obj->Set(1, Integer::New(dest[1])); // Run Status Indicator
	if (ret > 2) { // Additional data
		for (int i = 2; i < ret; i++) dest_obj->Set(i, Integer::New(dest[i]));
	}
	
	return Integer::New(ret);
}

// int modbus_write_bit(modbus_t *ctx, int addr, int status);
// Integer write_bit(External, Integer, Integer);
Handle<Value> WriteBit(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int status = Local<Integer>::Cast(args[2])->Int32Value();
	
	int ret = modbus_write_bit(ctx, addr, status);
	
	return Integer::New(ret);
}

// int modbus_write_register(modbus_t *ctx, int addr, int value);
// Integer write_register(External, Integer, Integer);
Handle<Value> WriteRegister(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int value = Local<Integer>::Cast(args[2])->Int32Value();
	
	int ret = modbus_write_register(ctx, addr, value);
	
	return Integer::New(ret);
}

// int modbus_write_bits(modbus_t *ctx, int addr, int nb, const uint8_t *src);
// Integer write_bits(External, Integer, Integer, Array);
Handle<Value> WriteBits(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> src_arr = Local<Array>::Cast(args[3]);
	
	uint8_t src[nb];
	for (int i = 0; i < nb; i++) src[i] = src_arr->Get(i)->Uint32Value();
	
	int ret = modbus_write_bits(ctx, addr, nb, src);
	
	return Integer::New(ret);
}

// int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *src);
// Integer write_registers(External, Integer, Integer, Array);
Handle<Value> WriteRegisters(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int addr = Local<Integer>::Cast(args[1])->Int32Value();
	int nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> src_arr = Local<Array>::Cast(args[3]);
	
	uint16_t src[nb];
	for (int i = 0; i < nb; i++) src[i] = src_arr->Get(i)->Uint32Value();
	
	int ret = modbus_write_registers(ctx, addr, nb, src);
	
	return Integer::New(ret);
}

// int modbus_write_and_read_registers(modbus_t *ctx, int write_addr, int write_nb, const uint16_t *src, int read_addr, int read_nb, const uint16_t *dest);
// Integer write_and_read_registers(External, Integer, Integer, Array, Integer, Integer, Array);
Handle<Value> WriteAndReadRegisters(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int write_addr = Local<Integer>::Cast(args[1])->Int32Value();
	int write_nb = Local<Integer>::Cast(args[2])->Int32Value();
	Local<Array> src_arr = Local<Array>::Cast(args[3]);
	int read_addr = Local<Integer>::Cast(args[4])->Int32Value();
	int read_nb = Local<Integer>::Cast(args[5])->Int32Value();
	Local<Array> dest_arr = Local<Array>::Cast(args[6]);
	
	uint16_t src[write_nb];
	for (int i = 0; i < write_nb; i++) src[i] = src_arr->Get(i)->Uint32Value();
	
	uint16_t dest[read_nb];
	
	int ret = modbus_write_and_read_registers(ctx,
		write_addr, write_nb, src,
		read_addr, read_nb, dest);
	
	for (int i = 0; i < read_nb; i++) dest_arr->Set(i, Number::New(dest[i]));
	
	return Integer::New(ret);
}

//int modbus_send_raw_request(modbus_t *ctx, uint8_t *raw_req, int raw_req_length);
// Integer send_raw_request(External, Array, Integer);
Handle<Value> SendRawRequest(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Array> raw_req_arr = Local<Array>::Cast(args[1]);
	int raw_req_length = Local<Integer>::Cast(args[2])->Int32Value();
	
	uint8_t raw_req[raw_req_length];
	for (int i = 0; i < raw_req_length; i++) raw_req[i] = raw_req_arr->Get(i)->Uint32Value();
	
	int ret = modbus_send_raw_request(ctx, raw_req, raw_req_length);
	
	return Integer::New(ret);
}

// int modbus_receive_confirmation(modbus_t *ctx, uint8_t *rsp);
// Integer receive_confirmation(External, Array);
Handle<Value> ReceiveConfirmation(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Array> rsp_arr = Local<Array>::Cast(args[1]);
	
	uint8_t rsp[MODBUS_TCP_MAX_ADU_LENGTH];
    memset(rsp, 0, MODBUS_TCP_MAX_ADU_LENGTH * sizeof(uint8_t));
	
	int ret = modbus_receive_confirmation(ctx, rsp);
	
	if (ret > 0) {
		for (int i = 0; i < ret; i++) rsp_arr->Set(i, Number::New(rsp[i]));
	}
	
	return Integer::New(ret);
}

// int modbus_reply_exception(modbus_t *ctx, const uint8_t *req, unsigned int exception_code);
// Integer reply_exception(External, Array, Integer);
Handle<Value> ReplyException(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Array> req_arr = Local<Array>::Cast(args[1]);
	unsigned int exception_code = Local<Integer>::Cast(args[2])->Int32Value();
	
	int req_arr_len = req_arr->InternalFieldCount();
	uint8_t req[req_arr_len];
	for (int i = 0; i < req_arr_len; i++) req[i] = req_arr->Get(i)->Uint32Value();
	
	int ret = modbus_reply_exception(ctx, req, exception_code);
	
	return Integer::New(ret);
}

// modbus_mapping_t *modbus_mapping_new(int nb_bits, int nb_input_bits, int nb_registers, int nb_input_registers);
// External mapping_new(Integer, Integer, Integer, Integer);
Handle<Value> MappingNew(const Arguments& args) {
	int nb_bits = Local<Integer>::Cast(args[0])->Int32Value();
	int nb_input_bits = Local<Integer>::Cast(args[1])->Int32Value();
	int nb_registers = Local<Integer>::Cast(args[2])->Int32Value();
	int nb_input_registers = Local<Integer>::Cast(args[3])->Int32Value();
	
	modbus_mapping_t *map = modbus_mapping_new(nb_bits, nb_input_bits, nb_registers, nb_input_registers);
	
	if (map == NULL) return Null();
	
	return External::New(map);
}

// void modbus_mapping_free(modbus_mapping_t *mb_mapping);
// Undefined mapping_free(External);
Handle<Value> MappingFree(const Arguments& args) {
	modbus_mapping_t *map = static_cast<modbus_mapping_t *>(External::Cast(*args[0])->Value());
	
	modbus_mapping_free(map);
	
	return Undefined();
}

// int modbus_receive(modbus_t *ctx, uint8_t *req);
// Integer receive(External, Array);
Handle<Value> Receive(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Array> req_arr = Local<Array>::Cast(args[1]);
	
	uint8_t req[MODBUS_TCP_MAX_ADU_LENGTH];
    memset(req, 0, MODBUS_TCP_MAX_ADU_LENGTH * sizeof(uint8_t));
	
	int ret = modbus_receive(ctx, req);
	
	if (ret > 0) {
		for (int i = 0; i < ret; i++) req_arr->Set(i, Number::New(req[i]));
	}
	
	return Integer::New(ret);
}

// int modbus_reply(modbus_t *ctx, const uint8_t *req, int req_length, modbus_mapping_t *mb_mapping);
// Integer reply(External, Array, Integer, External);
Handle<Value> Reply(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Array> req_arr = Local<Array>::Cast(args[1]);
	int req_length = Local<Integer>::Cast(args[2])->Int32Value();
	modbus_mapping_t *mb_mapping = static_cast<modbus_mapping_t *>(External::Cast(*args[3])->Value());
	
	uint8_t req[req_length];
	for (int i = 0; i < req_length; i++) req[i] = req_arr->Get(i)->Uint32Value();
	
	int ret = modbus_reply(ctx, req, req_length, mb_mapping);
	
	return Integer::New(ret);
}

// const char *modbus_strerror(*int errnum);
// String strerror();
Handle<Value> Strerror(const Arguments& args) {
	const char *ret = modbus_strerror(errno);
	
	return String::New(ret);
}

// int modbus_tcp_listen(modbus_t *ctx, int nb_connection);
// Integer tcp_listen(External, Integer);
Handle<Value> TcpListen(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int nb_connection = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_tcp_listen(ctx, nb_connection);
	
	return Integer::New(ret);
}

// int modbus_tcp_accept(modbus_t *ctx, int *socket);
// Integer tcp_accept(External, Integer);
Handle<Value> TcpAccept(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int socket = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_tcp_accept(ctx, &socket);
	
	return Integer::New(ret);
}

// int modbus_tcp_pi_listen(modbus_t *ctx, int nb_connection);
// Integer tcp_pi_listen(External, Integer);
Handle<Value> js_tcp_pi_listen(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int nb_connection = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_tcp_pi_listen(ctx, nb_connection);
	
	return Integer::New(ret);
}

// int modbus_tcp_pi_accept(modbus_t *ctx, int *socket);
// Integer tcp_pi_accept(External, Integer);
Handle<Value> TcpPiAccept(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int socket = Local<Integer>::Cast(args[1])->Int32Value();
	
	int ret = modbus_tcp_pi_accept(ctx, &socket);
	
	return Integer::New(ret);
}

// convert modbus_mapping_t* to json object
// Undefined map_to_json(External, Object);
Handle<Value> MapToJson(const Arguments& args) {
	modbus_mapping_t *map = static_cast<modbus_mapping_t *>(External::Cast(*args[0])->Value());
	Local<Object> jso = Local<Object>::Cast(args[1]);
	
	jso->Set(String::New("nb_bits"), Integer::New(map->nb_bits));
	jso->Set(String::New("nb_input_bits"), Integer::New(map->nb_input_bits));
	jso->Set(String::New("nb_input_registers"), Integer::New(map->nb_input_registers));
	jso->Set(String::New("nb_registers"), Integer::New(map->nb_registers));
	
	Local<Array> tab_bits = Array::New();
	for (int i = 0; i < map->nb_bits; i++) {
		tab_bits->Set(i, Integer::New(map->tab_bits[i]));
	}
	
	Local<Array> tab_input_bits = Array::New();
	for (int i = 0; i < map->nb_input_bits; i++) {
		tab_input_bits->Set(i, Integer::New(map->tab_input_bits[i]));
	}
	
	Local<Array> tab_input_registers = Array::New();
	for (int i = 0; i < map->nb_input_registers; i++) {
		tab_input_registers->Set(i, Integer::New(map->tab_input_registers[i]));
	}
	
	Local<Array> tab_registers = Array::New();
	for (int i = 0; i < map->nb_registers; i++) {
		tab_registers->Set(i, Integer::New(map->tab_registers[i]));
	}
	
	jso->Set(String::New("tab_bits"), tab_bits);
	jso->Set(String::New("tab_input_bits"), tab_input_bits);
	jso->Set(String::New("tab_input_registers"), tab_input_registers);
	jso->Set(String::New("tab_registers"), tab_registers);
	
	return Undefined();
}

// convert json object to modbus_mapping_t*
// Undefined json_to_map(Object, External);
Handle<Value> JsonToMap(const Arguments& args) {
	Local<Object> jso = Local<Object>::Cast(args[0]);
	modbus_mapping_t *map = static_cast<modbus_mapping_t *>(External::Cast(*args[1])->Value());
	
	map->nb_bits = jso->Get(String::New("nb_bits"))->Int32Value();
	map->nb_input_bits = jso->Get(String::New("nb_input_bits"))->Int32Value();
	map->nb_input_registers = jso->Get(String::New("nb_input_registers"))->Int32Value();
	map->nb_registers = jso->Get(String::New("nb_registers"))->Int32Value();
	
	Local<Array> tab_bits = Local<Array>::Cast(jso->Get(String::New("tab_bits")));
	for (int i = 0; i < map->nb_bits; i++) {
		map->tab_bits[i] = tab_bits->Get(i)->Int32Value();
	}
	
	Local<Array> tab_input_bits = Local<Array>::Cast(jso->Get(String::New("tab_input_bits")));
	for (int i = 0; i < map->nb_input_bits; i++) {
		map->tab_input_bits[i] = tab_input_bits->Get(i)->Int32Value();
	}
	
	Local<Array> tab_input_registers = Local<Array>::Cast(jso->Get(String::New("tab_input_registers")));
	for (int i = 0; i < map->nb_input_registers; i++) {
		map->tab_input_registers[i] = tab_input_registers->Get(i)->Int32Value();
	}
	
	Local<Array> tab_registers = Local<Array>::Cast(jso->Get(String::New("tab_registers")));
	for (int i = 0; i < map->nb_registers; i++) {
		map->tab_registers[i] = tab_registers->Get(i)->Int32Value();
	}
	
	return Undefined();
}

struct tcp_accept_t {
    modbus_t *ctx;
    int socket;
    Persistent<Function> cb;
    int ret;
};

void tcp_accept_w(uv_work_t* req) {
    tcp_accept_t* request = (tcp_accept_t*)req->data;
    request->ret = modbus_tcp_accept(request->ctx, &(request->socket));
}

void tcp_accept_a(uv_work_t* req, int i) {
    HandleScope scope;
	
    tcp_accept_t* request = (tcp_accept_t*)req->data;
    delete req;
	
    Handle<Value> argv[1];
    argv[0] = Integer::New(request->ret);
	
    request->cb->Call(Context::GetCurrent()->Global(), 1, argv);
	
    request->cb.Dispose();
    delete request;
}

// Undefined tcp_accept_async(External, Integer, Function);
// callback function - Function(Integer);
Handle<Value> TcpAcceptAsync(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	int socket = Local<Integer>::Cast(args[1])->Int32Value();
	Local<Function> cb = Local<Function>::Cast(args[2]);
	
	tcp_accept_t* request = new tcp_accept_t;
	request->ctx = ctx;
	request->socket = socket;
	request->cb = Persistent<Function>::New(cb);
	
	uv_work_t* req = new uv_work_t();
	req->data = request;
	
	uv_queue_work(uv_default_loop(), req, tcp_accept_w, tcp_accept_a);
	
	return Undefined();
}

struct receive_t {
    modbus_t *ctx;
    uint8_t *req;
    int len;
    Persistent<Function> cb;
};

void receive_w(uv_work_t* req) {
    receive_t* request = (receive_t*)req->data;
    request->len = modbus_receive(request->ctx, request->req);
}

void receive_a(uv_work_t* req, int i) {
    HandleScope scope;
	
    receive_t* request = (receive_t*)req->data;
    delete req;
	
	int len = request->len;
	
	Local<Array> req_arr = Array::New();
	if (len > 0) {
		for (int i = 0; i < len; i++) req_arr->Set(i, Number::New(request->req[i]));
	}
	
	delete request->req;
	
    Handle<Value> argv[2];
    argv[0] = req_arr;
    argv[1] = Integer::New(len);
    
    request->cb->Call(Context::GetCurrent()->Global(), 2, argv);
	
    request->cb.Dispose();
    delete request;
}

// Undefined receive_async(External, Function);
// callback function - Function(Array, Integer);
Handle<Value> ReceiveAsync(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Function> cb = Local<Function>::Cast(args[1]);
	
	uint8_t *mbreq = new uint8_t[MODBUS_TCP_MAX_ADU_LENGTH];;
    memset(mbreq, 0, MODBUS_TCP_MAX_ADU_LENGTH * sizeof(uint8_t));
	
	receive_t* request = new receive_t;
	request->ctx = ctx;
	request->req = mbreq;
	request->len = 0;
	request->cb = Persistent<Function>::New(cb);
	
	uv_work_t* req = new uv_work_t();
	req->data = request;
	
	uv_queue_work(uv_default_loop(), req, receive_w, receive_a);
	
	return Undefined();
}

struct connect_t {
    modbus_t *ctx;
    Persistent<Function> cb;
    int ret;
};

void connect_w(uv_work_t* req) {
    connect_t* request = (connect_t*)req->data;
    request->ret = modbus_connect(request->ctx);
}

void connect_a(uv_work_t* req, int i) {
    HandleScope scope;
	
    connect_t* request = (connect_t*)req->data;
    delete req;
	
    Handle<Value> argv[1];
    argv[0] = Integer::New(request->ret);
    
    request->cb->Call(Context::GetCurrent()->Global(), 1, argv);
	
    request->cb.Dispose();
    delete request;
}

// Undefined connect_async(External, Function);
// callback function - Function(Integer);
Handle<Value> ConnectAsync(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	Local<Function> cb = Local<Function>::Cast(args[1]);
	
	connect_t* request = new connect_t;
	request->ctx = ctx;
	request->cb = Persistent<Function>::New(cb);
	request->ret = 0;
	
	uv_work_t* req = new uv_work_t();
	req->data = request;
	
	uv_queue_work(uv_default_loop(), req, connect_w, connect_a);
	
	return Undefined();
}

// закрыть из треда
// Undefined close_mt(External);
Handle<Value> CloseMt(const Arguments& args) {
	modbus_t *ctx = static_cast<modbus_t *>(External::Cast(*args[0])->Value());
	
	modbus_close_mt(ctx);
	
	return Undefined();
}

extern "C" {
void init (Handle<Object> target) {
	HandleScope scope;
	
	// constants
	target->Set(String::New("LIBMODBUS_VERSION_MAJOR"), Number::New(LIBMODBUS_VERSION_MAJOR));
	target->Set(String::New("LIBMODBUS_VERSION_MINOR"), Number::New(LIBMODBUS_VERSION_MINOR));
	target->Set(String::New("LIBMODBUS_VERSION_MICRO"), Number::New(LIBMODBUS_VERSION_MICRO));
	target->Set(String::New("LIBMODBUS_VERSION_STRING"), String::New(LIBMODBUS_VERSION_STRING));
	//target->Set(String::New("LIBMODBUS_VERSION_HEX"), Number::New(LIBMODBUS_VERSION_HEX)); bug in header
	
	target->Set(String::New("FALSE"), Number::New(FALSE));
	target->Set(String::New("TRUE"), Number::New(TRUE));
	
	target->Set(String::New("OFF"), Number::New(OFF));
	target->Set(String::New("ON"), Number::New(ON));
	
	target->Set(String::New("MODBUS_BROADCAST_ADDRESS"), Number::New(MODBUS_BROADCAST_ADDRESS));
	
	target->Set(String::New("MODBUS_MAX_READ_BITS"), Number::New(MODBUS_MAX_READ_BITS));
	target->Set(String::New("MODBUS_MAX_WRITE_BITS"), Number::New(MODBUS_MAX_WRITE_BITS));
	
	target->Set(String::New("MODBUS_MAX_READ_REGISTERS"), Number::New(MODBUS_MAX_READ_REGISTERS));
	target->Set(String::New("MODBUS_MAX_WRITE_REGISTERS"), Number::New(MODBUS_MAX_WRITE_REGISTERS));
	target->Set(String::New("MODBUS_MAX_RW_WRITE_REGISTERS"), Number::New(MODBUS_MAX_RW_WRITE_REGISTERS));
	
	target->Set(String::New("MODBUS_ENOBASE"), Number::New(MODBUS_ENOBASE));
	
	target->Set(String::New("MODBUS_EXCEPTION_ILLEGAL_FUNCTION"), Number::New(MODBUS_EXCEPTION_ILLEGAL_FUNCTION));
	target->Set(String::New("MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS"), Number::New(MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS));
	target->Set(String::New("MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE"), Number::New(MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE));
	target->Set(String::New("MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE"), Number::New(MODBUS_EXCEPTION_SLAVE_OR_SERVER_FAILURE));
	target->Set(String::New("MODBUS_EXCEPTION_ACKNOWLEDGE"), Number::New(MODBUS_EXCEPTION_ACKNOWLEDGE));
	target->Set(String::New("MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY"), Number::New(MODBUS_EXCEPTION_SLAVE_OR_SERVER_BUSY));
	target->Set(String::New("MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE"), Number::New(MODBUS_EXCEPTION_NEGATIVE_ACKNOWLEDGE));
	target->Set(String::New("MODBUS_EXCEPTION_MEMORY_PARITY"), Number::New(MODBUS_EXCEPTION_MEMORY_PARITY));
	target->Set(String::New("MODBUS_EXCEPTION_NOT_DEFINED"), Number::New(MODBUS_EXCEPTION_NOT_DEFINED));
	target->Set(String::New("MODBUS_EXCEPTION_GATEWAY_PATH"), Number::New(MODBUS_EXCEPTION_GATEWAY_PATH));
	target->Set(String::New("MODBUS_EXCEPTION_GATEWAY_TARGET"), Number::New(MODBUS_EXCEPTION_GATEWAY_TARGET));
	target->Set(String::New("MODBUS_EXCEPTION_MAX"), Number::New(MODBUS_EXCEPTION_MAX));
	
	target->Set(String::New("EMBXILFUN"), Number::New(EMBXILFUN));
	target->Set(String::New("EMBXILADD"), Number::New(EMBXILADD));
	target->Set(String::New("EMBXILVAL"), Number::New(EMBXILVAL));
	target->Set(String::New("EMBXSFAIL"), Number::New(EMBXSFAIL));
	target->Set(String::New("EMBXACK"), Number::New(EMBXACK));
	target->Set(String::New("EMBXSBUSY"), Number::New(EMBXSBUSY));
	target->Set(String::New("EMBXNACK"), Number::New(EMBXNACK));
	target->Set(String::New("EMBXMEMPAR"), Number::New(EMBXMEMPAR));
	target->Set(String::New("EMBXGPATH"), Number::New(EMBXGPATH));
	target->Set(String::New("EMBXGTAR"), Number::New(EMBXGTAR));
	
	target->Set(String::New("EMBBADCRC"), Number::New(EMBBADCRC));
	target->Set(String::New("EMBBADDATA"), Number::New(EMBBADDATA));
	target->Set(String::New("EMBBADEXC"), Number::New(EMBBADEXC));
	target->Set(String::New("EMBUNKEXC"), Number::New(EMBUNKEXC));
	target->Set(String::New("EMBMDATA"), Number::New(EMBMDATA));
	
	target->Set(String::New("MODBUS_ERROR_RECOVERY_NONE"), Number::New(MODBUS_ERROR_RECOVERY_NONE));
	target->Set(String::New("MODBUS_ERROR_RECOVERY_LINK"), Number::New(MODBUS_ERROR_RECOVERY_LINK));
	target->Set(String::New("MODBUS_ERROR_RECOVERY_PROTOCOL"), Number::New(MODBUS_ERROR_RECOVERY_PROTOCOL));
	
	target->Set(String::New("MODBUS_RTU_MAX_ADU_LENGTH"), Number::New(MODBUS_RTU_MAX_ADU_LENGTH));
	target->Set(String::New("MODBUS_RTU_RS232"), Number::New(MODBUS_RTU_RS232));
	target->Set(String::New("MODBUS_RTU_RS485"), Number::New(MODBUS_RTU_RS485));
	
	target->Set(String::New("MODBUS_TCP_DEFAULT_PORT"), Number::New(MODBUS_TCP_DEFAULT_PORT));
	target->Set(String::New("MODBUS_TCP_SLAVE"), Number::New(MODBUS_TCP_SLAVE));
	target->Set(String::New("MODBUS_TCP_MAX_ADU_LENGTH"), Number::New(MODBUS_TCP_MAX_ADU_LENGTH));
	
	// functions
        NODE_SET_METHOD(target, "new_rtu", NewRtu);
        NODE_SET_METHOD(target, "rtu_get_serial_mode", RtuGetSerialMode);
        NODE_SET_METHOD(target, "rtu_set_serial_mode", RtuSetSerialMode);
	//target->Set(String::New("rtu_get_rts"), FunctionTemplate::New(js_rtu_get_rts)->GetFunction());
	//target->Set(String::New("rtu_set_rts"), FunctionTemplate::New(js_rtu_set_rts)->GetFunction());
        
        NODE_SET_METHOD(target, "new_tcp", NewTcp);
        NODE_SET_METHOD(target, "new_tcp_pi", NewTcpPi);
	
	NODE_SET_METHOD(target, "free", Free);
	
	NODE_SET_METHOD(target, "get_byte_timeout", GetByteTimeout);
	NODE_SET_METHOD(target, "set_byte_timeout", SetByteTimeout);
	NODE_SET_METHOD(target, "set_debug", SetDebug);
        NODE_SET_METHOD(target, "set_error_recovery", SetErrorRecovery);
	NODE_SET_METHOD(target, "get_header_length", GetHeaderLength);
	NODE_SET_METHOD(target, "get_response_timeout", GetResponseTimeout);
	NODE_SET_METHOD(target, "set_response_timeout", SetResponseTimeout);
	NODE_SET_METHOD(target, "set_slave", SetSlave);
	NODE_SET_METHOD(target, "set_socket", SetSocket);
	NODE_SET_METHOD(target, "get_socket", GetSocket);
	
	NODE_SET_METHOD(target, "connect", Connect);
	NODE_SET_METHOD(target, "close", Close);
	NODE_SET_METHOD(target, "flush", Flush);
	
	NODE_SET_METHOD(target, "read_bits", ReadBits);
	NODE_SET_METHOD(target, "read_input_bits", ReadInputBits);
	NODE_SET_METHOD(target, "read_registers", ReadRegisters);
	NODE_SET_METHOD(target, "read_input_registers", ReadInputRegisters);
	NODE_SET_METHOD(target, "report_slave_id", ReportSlaveId);
	NODE_SET_METHOD(target, "write_bit", WriteBit);
	NODE_SET_METHOD(target, "write_register", WriteRegister);
	NODE_SET_METHOD(target, "write_bits", WriteBits);
	NODE_SET_METHOD(target, "write_registers", WriteRegisters);
	NODE_SET_METHOD(target, "write_and_read_registers", WriteAndReadRegisters);
	NODE_SET_METHOD(target, "send_raw_request", SendRawRequest);
	NODE_SET_METHOD(target, "receive_confirmation", ReceiveConfirmation);
	NODE_SET_METHOD(target, "reply_exception", ReplyException);
	
	NODE_SET_METHOD(target, "mapping_new", MappingNew);
	NODE_SET_METHOD(target, "mapping_free", MappingFree);
	NODE_SET_METHOD(target, "receive", Receive);
	NODE_SET_METHOD(target, "reply", Reply);
	
	NODE_SET_METHOD(target, "strerror", Strerror);
	
	NODE_SET_METHOD(target, "tcp_listen", TcpListen);
	NODE_SET_METHOD(target, "tcp_accept", TcpAccept);
	NODE_SET_METHOD(target, "tcp_pi_listen", TcpPiAccept);
	NODE_SET_METHOD(target, "tcp_pi_accept", TcpPiAccept);
	
	// my functions
	NODE_SET_METHOD(target, "map_to_json", MapToJson);
	NODE_SET_METHOD(target, "json_to_map", JsonToMap);
	
	NODE_SET_METHOD(target, "tcp_accept_async", TcpAcceptAsync);
	NODE_SET_METHOD(target, "receive_async", ReceiveAsync);
	NODE_SET_METHOD(target, "connect_async", ConnectAsync);
	NODE_SET_METHOD(target, "close_mt", CloseMt);
}

NODE_MODULE(modbus_binding, init)
}
