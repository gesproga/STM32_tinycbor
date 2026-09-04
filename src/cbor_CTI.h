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


typedef enum{
	CBOR_ACCION_NO_RECONOCIDA = 0,
	CBOR_ACCION_READ,
	CBOR_ACCION_WRITE,
	CBOR_ACCION_WRITE_READ,
	CBOR_ACCION_RT_READ,
	CBOR_ACCION_RT_WRITE,
	CBOR_ACCION_RT_ERROR,
	CBOR_ACCION_EVENTO,

}EN_CBOR_ACCION;

#define DEF_CBOR_ACCION_READ 	"R"	//peticion de lectura
#define DEF_CBOR_ACCION_WRITE 	"W"	//envia datos escritura
#define DEF_CBOR_ACCION_RT_READ "RES"	//respuesta lectura escritura
#define DEF_CBOR_ACCION_RT_WRITE "ACK"	//respuesta ACK
#define DEF_CBOR_ACCION_RT_ERROR "E"	//respuesta ERROR
#define DEF_CBOR_ACCION_WRITE_READ "WR"	//respuesta ERROR
#define DEF_CBOR_ACCION_EVENTO "EV"	//evento asincrono



#define DEF_CBOR_CLAVE_PRINCIPAL_STU 	"STU\0"
#define DEF_CBOR_CLAVE_PRINCIPAL_CFG 	"CFG\0"
#define DEF_CBOR_CLAVE_PRINCIPAL_EVT 	"EVT\0"
#define DEF_CBOR_CLAVE_PRINCIPAL_ERR 	"ERR\0"

//typedef struct{
//
//	puint32 clave_equipo;
//
//	puint8 servidor_conectar;
//	pstring_32 direccion_servidor;
//
//
//}ST_apli_lib;

#define DEF_CBOR_ACCION_RT_READ_LEN 5
#define DEF_CBOR_ACCION_RT_WRITE_LEN 5

typedef struct
{
	CborParser parser;
	CborValue decoder_map;
	CborValue array_raiz;

	CborTag tag;

	EN_CBOR_ACCION accion_tipo;

	int id;

	int dr_array[DEF_CBOR_ACCION_RT_WRITE_LEN];
	size_t dr_count;

	CborValue map_CFG;
	CborValue map_STU;
	CborValue map_EVT;

	CTI_SI_NO parser_iniciado; //indica si se ha iniciado el parser


}ST_CBOR_tipo_rx_decoder;






typedef struct
{

	//si configuramos estas funciones, las llamara cuando este montando la trama
	 void (*fun_CFG)(CborEncoder *mapa_datos); // función para rellenar el mapa
	 CborError (*fun_STU)(CborEncoder *mapa_datos); // función para rellenar el mapa
	 void (*fun_EVT)(CborEncoder *mapa_datos); // función para rellenar el mapa
	 void (*fun_ERR)(CborEncoder *mapa_datos); // función para rellenar el mapa


	 //configuracion informacion respuesta
	CborTag tag;
	EN_CBOR_ACCION accion_tipo;
	int id;
	int dr_array[DEF_CBOR_ACCION_RT_WRITE_LEN];
	size_t dr_count;

	//encoder donde se monta la trama
	CborEncoder encoder;
	CborEncoder array_raiz;



	//donde se monta buffer
	uint8_t *buffer_montar_rt;
	size_t size_max_buffer_montar_rt;


}ST_CBOR_tipo_tx_encoder;

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//funciones encoder
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//se tiene que llamar al principio para inicializar el encoder. esto crea un array de tag.
//de esta forma podemos enviar diferentes tag en una misma trama
CborError cbor_CTI_set_init_array_encode(ST_CBOR_tipo_tx_encoder *st_encoder,uint8_t *buffer_montar, size_t size_max_buffer_montar);




//se tiene que llamar al final cuando se ha finalizado el encoder.
//esto finaliza el array de tag y retorna el tamaño del buffer montado
CborError  cbor_CTI_set_fin_array_encode(ST_CBOR_tipo_tx_encoder *st_encoder,size_t *size_buffer_montar);



//cada tag que queramos llamar dentro del encoder llamamos a cbor_CTI_set_encabezado_encode.
//antes de llamar a esta funcion deberiamos crear st_encoder y iniciar las variables de configuracion que deseeemos
//ejemplo:
	//ST_CBOR_tipo_tx_encoder st_encoder = {0};
	//st_encoder.tag = parser_decoder->tag;
	//st_encoder.accion_tipo = CBOR_ACCION_RT_READ;
	//st_encoder.id = parser_decoder->id;
	//st_encoder.fun_STU=APIC_Placa_base_cbor_STU_encode;
	//memcpy(&st_encoder.dr_array, &parser_decoder.dr_array, sizeof(st_encoder->dr_array));

CborError  cbor_CTI_set_encabezado_encode(ST_CBOR_tipo_tx_encoder *st_encoder);











/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//se llama cuando tengamos informacion CBOR que decodificar.
//importante iniciar parser decoder ST_CBOR_tipo_rx_decoder parser_decoder={0};
//como se pueden enviar múltiples tag dentro de una trama, se puede llamar varias veces a esta función hasta que se hayan decodificado todos los tag.
//retorna CborErrorAdvancePastEOF si no hay mas datos que leer
//toda la informacion decodificada se guarda en parser_decoder.
CborError  cbor_CTI_get_encabezado_decode(
		uint8_t *buffer_rx,size_t size,
		ST_CBOR_tipo_rx_decoder *parser_decoder
);









//funcion para obtener el valor de un float
CborError  cbor_CTI_value_get_float(
		const CborValue *value, float *result
);



CborError cbor_CTI_encode_float(CborEncoder *encoder, float value);


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

