#ifndef __CBOR_CTI__
#define __CBOR_CTI__

/*
 *
 * 
 */
#include "define_LIB.h"
#include "cbor.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTAS





//////////////////////////////////////////////////////////////////////////////////////////////////////////////




#define DEF_CBOR_ACCION_READ "R"	//peticion de lectura
#define DEF_CBOR_ACCION_WRITE "W"	//envia datos escritura
#define DEF_CBOR_ACCION_RT_READ "RES"	//respuesta lectura escritura
#define DEF_CBOR_ACCION_RT_WRITE "ACK"	//respuesta ACK
#define DEF_CBOR_ACCION_RT_ERROR "E"	//respuesta ERROR


//typedef struct{
//
//	puint32 clave_equipo;
//
//	puint8 servidor_conectar;
//	pstring_32 direccion_servidor;
//
//
//}ST_apli_lib;




//extern ST_apli_lib st_apli_lib;


/********************************************************
//include
*********************************************************/



/********************************************************
//DEFINE
*********************************************************/

/**********************************************************

//ESTRUCTURAS

***********************************************************/


/********************************************************
//FUNCIONES entrada
*********************************************************/

//ejemplo de uso

//1001({
//  "accion": "R",
//  "ID": 42,
//  "datos": {
//    "nombre": "Sensor A",
//    "temperatura": 24.7
//  }
//})

//uint8_t buffer[256];
//CborEncoder encoder, root_map, datos_map;
//
//cbor_encoder_init(&encoder, buffer, sizeof(buffer), 0);
//
//// Crear encabezado con tag 1001, accion = "R", ID = 42
//cbor_encode_encabezado_tag_accion_id(&encoder, &root_map, 1001, "R", 42);

//// Ahora puedes abrir el submapa "datos"
//cbor_encoder_create_map(&root_map, &datos_map, 2);
//
//cbor_encode_text_stringz(&datos_map, "nombre");
//cbor_encode_text_stringz(&datos_map, "Sensor A");
//
//cbor_encode_text_stringz(&datos_map, "temperatura");
//cbor_encode_float(&datos_map, 24.7f);
//
//cbor_encoder_close_container(&root_map, &datos_map);  // cerrar "datos"
//cbor_encoder_close_container(&encoder, &root_map);    // cerrar mapa raíz

CborError  cbor_CTI_init_encabezado_encode(
		uint8_t *buffer_montar, size_t size,
		CborEncoder* encoder,
	    CborEncoder* out_map,
	    uint64_t tag,
		const char* accion,
	    int id);



//se llama cuando termina de escribir los datos
CborError  cbor_CTI_fin_encabezado_encode(CborEncoder* encoder,CborEncoder* datos);










CborError  cbor_CTI_init_encabezado_decode(
		uint8_t *buffer_rx,size_t size,
		CborParser *parser,	CborValue *decoder,	CborValue *map_datos,
		CborTag *tag,
		char* accion,size_t *accion_len,
	    int *id);
/********************************************************
//FUNCIONES salida
*********************************************************/


/*VARIABLES*********************************************************************************************/ 


#endif
/********************************************************
//NOTAS DE FUNCIONAMIENTO
*********************************************************
* 
*
*********************************************************/

