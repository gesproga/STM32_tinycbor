/*
 * 
 */

//////////////////////////////////////////////

#include "cbor_CTI.h"

/*DEFINE***************************************************************************************/


/*STRUCT***************************************************************************************/


/*VARIABLES*********************************************************************************************/ 

struct {
//	CborEncoder *encoder;
//	CborEncoder *map;


//	CborParser parser;
//	CborValue it;

}VA_cbor_CTI;



/*FUNCIONES**********************************************************************************/


/**********************************************************************************************/

CborError  cbor_CTI_set_init_array_encode(ST_CBOR_tipo_tx_encoder *st_encoder,uint8_t *buffer_montar, size_t size_max_buffer_montar)
{
	CborError err=0;

	st_encoder->buffer_montar_rt=buffer_montar;
	st_encoder->size_max_buffer_montar_rt=size_max_buffer_montar;

	cbor_encoder_init(&st_encoder->encoder, st_encoder->buffer_montar_rt, st_encoder->size_max_buffer_montar_rt, 0);
	// Crear mapa raíz
	err |= cbor_encoder_create_array(&st_encoder->encoder, &st_encoder->array_raiz, CborIndefiniteLength);
	return err;
}


CborError  cbor_CTI_set_fin_array_encode(ST_CBOR_tipo_tx_encoder *st_encoder,size_t *size_buffer_montar)
{
	CborError err=0;
	// Crear mapa raíz
	err |= cbor_encoder_close_container(&st_encoder->encoder, &st_encoder->array_raiz);


	if (cbor_encoder_get_buffer_size(&st_encoder->encoder, st_encoder->buffer_montar_rt)> st_encoder->size_max_buffer_montar_rt) {
		return CborErrorOutOfMemory; // Buffer insuficiente
	}

	// Actualizar el tamaño del buffer montado
	*size_buffer_montar= cbor_encoder_get_buffer_size(&st_encoder->encoder, st_encoder->buffer_montar_rt);


	return err;
}




CborError  cbor_CTI_set_encabezado_encode(
		//uint8_t *buffer_montar, size_t *size,
		ST_CBOR_tipo_tx_encoder *st_encoder)
{
	CborError err=0;
	CborEncoder mapa_raiz;

	//cbor_encoder_init(&st_encoder->encoder, buffer_montar, *size, 0);

	 // Codificar tag
	err = cbor_encode_tag(&st_encoder->array_raiz,st_encoder->tag);
	if (err != CborNoError) return err;

	// Crear mapa raíz con 3 entradas ("accion", "ID", "datos")
	err = cbor_encoder_create_map(&st_encoder->array_raiz, &mapa_raiz, CborIndefiniteLength);  //
	if (err != CborNoError) return err;

	 // "accion": "R" (por ejemplo)
	err = cbor_encode_text_stringz(&mapa_raiz, "AC");
	if (err != CborNoError) return err;

	// Determinar tipo de acción y codificar
	switch (st_encoder->accion_tipo) {
	case CBOR_ACCION_READ:err = cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_ACCION_READ);break;
	case CBOR_ACCION_WRITE:err = cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_ACCION_WRITE);break;
	case CBOR_ACCION_WRITE_READ:err = cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_ACCION_WRITE_READ);break;
	case CBOR_ACCION_RT_READ:err = cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_ACCION_RT_READ);break;
	case CBOR_ACCION_RT_WRITE:err = cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_ACCION_RT_WRITE);break;
	case CBOR_ACCION_RT_ERROR:err = cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_ACCION_RT_ERROR);break;
	default:
		return CborErrorIllegalType; // acción no válida
	}
	if (err != CborNoError) return err;


	// "ID": 42 (por ejemplo)
	err = cbor_encode_text_stringz(&mapa_raiz, "ID");
	if (err != CborNoError) return err;
	err = cbor_encode_int(&mapa_raiz, st_encoder->id);
	if (err != CborNoError) return err;


	// "DR": 1,2,3 (por ejemplo)
	err = cbor_encode_text_stringz(&mapa_raiz, "DR");
	if (err != CborNoError) return err;

	CborEncoder dr_array;
	err |= cbor_encoder_create_array(&mapa_raiz, &dr_array, CborIndefiniteLength);

	for (size_t i = 0; i < st_encoder->dr_count; ++i)
	{
	    err |= cbor_encode_int(&dr_array, st_encoder->dr_array[i]);
	}

	err |= cbor_encoder_close_container(&mapa_raiz, &dr_array);




	//comprobamos si tenemos que rellenar STU

	if (st_encoder->fun_STU != NULL)
	{
		err |= cbor_encode_text_stringz(&mapa_raiz,DEF_CBOR_CLAVE_PRINCIPAL_STU);
		CborEncoder mapa_temp;
		err |= cbor_encoder_create_map(&mapa_raiz, &mapa_temp,CborIndefiniteLength);
		st_encoder->fun_STU(&mapa_temp); // Llamar a la función que codifica los datos en el mapa
		err |= cbor_encoder_close_container(&mapa_raiz, &mapa_temp);
	}

	//comprobamos si tenemos que rellenar CFG
	if (st_encoder->fun_CFG != NULL) {
		err |= cbor_encode_text_stringz(&mapa_raiz,
				DEF_CBOR_CLAVE_PRINCIPAL_CFG);
		CborEncoder mapa_temp;
		err |= cbor_encoder_create_map(&mapa_raiz, &mapa_temp,
				CborIndefiniteLength);
		st_encoder->fun_CFG(&mapa_temp); // Llamar a la función que codifica los datos en el mapa
		err |= cbor_encoder_close_container(&mapa_raiz, &mapa_temp);
	}

	//comprobamos si tenemos que rellenar EVT
	if (st_encoder->fun_EVT != NULL) {
		err |= cbor_encode_text_stringz(&mapa_raiz,
		DEF_CBOR_CLAVE_PRINCIPAL_EVT);
		CborEncoder mapa_temp;
		err |= cbor_encoder_create_map(&mapa_raiz, &mapa_temp,
				CborIndefiniteLength);
		st_encoder->fun_EVT(&mapa_temp); // Llamar a la función que codifica los datos en el mapa
		err |= cbor_encoder_close_container(&mapa_raiz, &mapa_temp);
	}

	//comprobamos si tenemos que rellenar ERR
	if (st_encoder->fun_ERR != NULL) {
		err |= cbor_encode_text_stringz(&mapa_raiz,
		DEF_CBOR_CLAVE_PRINCIPAL_ERR);
		CborEncoder mapa_temp;
		err |= cbor_encoder_create_map(&mapa_raiz, &mapa_temp,
				CborIndefiniteLength);
		st_encoder->fun_ERR(&mapa_temp); // Llamar a la función que codifica los datos en el mapa
		err |= cbor_encoder_close_container(&mapa_raiz, &mapa_temp);
	}

	// Cerrar el mapa raíz
	err |= cbor_encoder_close_container(&st_encoder->array_raiz, &mapa_raiz);
	if (err != CborNoError) return err;
	// Finalizar la codificación
//	if (cbor_encoder_get_buffer_size(&st_encoder->encoder, buffer_montar)> *size) {
//		return CborErrorOutOfMemory; // Buffer insuficiente
//	}
//
//	// Actualizar el tamaño del buffer montado
//	*size = cbor_encoder_get_buffer_size(&st_encoder->encoder, buffer_montar);


	return err;  // listo para crear submapa con datos después
}




CborError  cbor_CTI_get_encabezado_decode(
		uint8_t *buffer_rx,size_t size,
		ST_CBOR_tipo_rx_decoder *parser_decoder
)
{
	//CborValue it;
	char accion[DEF_CBOR_ACCION_RT_READ_LEN]={0};
	size_t accion_len;
	CborError err;

	//CborValue array_raiz;

	CborValue map_datos;
	CborValue dr_array_it;
	uint8_t max_array=parser_decoder->dr_count;
	parser_decoder->dr_count = 0;
	if(parser_decoder->parser_iniciado==NO_c)
	{
		err = cbor_parser_init(buffer_rx, size, 0, &parser_decoder->parser, &parser_decoder->decoder_map);
        parser_decoder->parser_iniciado=SI_c;

        if (cbor_value_is_array(&parser_decoder->decoder_map)==0)
		{
			return CborErrorIllegalType; // No se esperaba un array aquí
		}
        err |= cbor_value_enter_container(&parser_decoder->decoder_map, &parser_decoder->array_raiz);
	}
	else {
		if (cbor_value_at_end(&parser_decoder->array_raiz))
		{
			return CborErrorAdvancePastEOF;
		}
	}

	// Leer tag
	if (cbor_value_is_tag(&parser_decoder->array_raiz)) {
		err |= cbor_value_get_tag(&parser_decoder->array_raiz, &parser_decoder->tag);
		err |= cbor_value_advance(&parser_decoder->array_raiz);
	}
	else
	{
		return CborErrorIllegalType; // No se encontró un tag válido
	}

	// Entrar al mapa raíz
	err |= cbor_value_enter_container(&parser_decoder->array_raiz, &map_datos);


	// Leer "AC"
	err |= cbor_value_advance(&map_datos); // clave "AC"
	accion_len=DEF_CBOR_ACCION_RT_READ_LEN;
	err |= cbor_value_copy_text_string(&map_datos, accion, &accion_len, &map_datos);


	//pasamos el tipo de acción a la estructura//////////////////////////////////////////////////////////////////////////

	if (memcmp(accion, DEF_CBOR_ACCION_WRITE_READ, accion_len) == 0)
		parser_decoder->accion_tipo = CBOR_ACCION_WRITE_READ; // acción válida para escribir

	if (memcmp(accion, DEF_CBOR_ACCION_READ, accion_len) == 0)
		parser_decoder->accion_tipo=CBOR_ACCION_READ; // acción no válida

	if (memcmp(accion, DEF_CBOR_ACCION_WRITE, accion_len) == 0)
		parser_decoder->accion_tipo = CBOR_ACCION_WRITE; // acción válida para escribir


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Leer "ID"
	err |= cbor_value_advance(&map_datos); // clave "ID"
	err |= cbor_value_get_int(&map_datos, &parser_decoder->id);
	err |= cbor_value_advance(&map_datos);


	// Leer "DR"

	if (cbor_value_is_array(&map_datos))
	{
	    err |= cbor_value_enter_container(&map_datos, &dr_array_it);
	    while (!cbor_value_at_end(&dr_array_it) && parser_decoder->dr_count < 5) {
	        int val = 0;
	        err |= cbor_value_get_int(&dr_array_it, &val);
	        if(parser_decoder->dr_count < max_array)
	        {
	        	parser_decoder->dr_array[parser_decoder->dr_count] = val;
	        	(parser_decoder->dr_count)++;
	        }
	        err |= cbor_value_advance(&dr_array_it);
	    }
	    err |= cbor_value_leave_container(&map_datos, &dr_array_it);
	}


	// Leer "CFG", "STU" y "EVT"
	while (!cbor_value_at_end(&map_datos))
	{
	    if (cbor_value_is_text_string(&map_datos))
	    {
	        char clave[8];
	        size_t len = sizeof(clave);
	        CborValue temp = map_datos;

	        if (cbor_value_copy_text_string(&temp, clave, &len, NULL) == CborNoError)
	        {
	            if (strcmp(clave, "CFG") == 0)
	            {
	                cbor_value_advance(&map_datos); // saltar clave
	                parser_decoder->map_CFG = map_datos;         // devolver puntero posicionado en el mapa
	                cbor_value_advance(&map_datos); // saltar valor
	                continue;
	            }
	            if (strcmp(clave, "EVT") == 0)
	            {
	                cbor_value_advance(&map_datos);
	                parser_decoder->map_EVT = map_datos;
	                cbor_value_advance(&map_datos);
	                continue;
	            }
	            if (strcmp(clave, "STU") == 0)
	            {
	                cbor_value_advance(&map_datos);
	                parser_decoder->map_STU = map_datos;
	                cbor_value_advance(&map_datos);
	                continue;
	            }

	        }
	    }
	    cbor_value_advance(&map_datos); // avanzar por seguridad
	}


	// 8. Salir del mapa raíz
	//err = cbor_value_leave_container(&array_raiz, &map_datos);
	err = cbor_value_advance(&parser_decoder->array_raiz);

	if (err != CborNoError) return err;



	return err;  //

}

/*notas ediciones*-----------------------------------------------------------------
*
*  
*
*END------------------------------------------------------------------*/
/*Posibles errores*-----------------------------------------------------------------
*
*  
*   
*END------------------------------------------------------------------*/





