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


CborError  cbor_CTI_init_encabezado_encode(
		uint8_t *buffer_montar, size_t size,
		CborEncoder* encoder,
	    CborEncoder* datos,
	    uint64_t tag,
		const char* accion,
	    int id,
		int dr)
{
	CborError err;


	cbor_encoder_init(encoder, buffer_montar, size, 0);

	 // Codificar tag
	err = cbor_encode_tag(encoder, tag);
	if (err != CborNoError) return err;

	// Crear mapa raíz con 3 entradas ("accion", "ID", "datos")
	err = cbor_encoder_create_map(encoder, datos, 4);  // 3 claves esperadas
	if (err != CborNoError) return err;

	 // "accion": "R" (por ejemplo)
	err = cbor_encode_text_stringz(datos, "AC");
	if (err != CborNoError) return err;
	err = cbor_encode_text_stringz(datos, accion);
	if (err != CborNoError) return err;


	// "ID": 42 (por ejemplo)
	err = cbor_encode_text_stringz(datos, "ID");
	if (err != CborNoError) return err;
	err = cbor_encode_int(datos, id);
	if (err != CborNoError) return err;


	// "ID": 42 (por ejemplo)
	err = cbor_encode_text_stringz(datos, "DR");
	if (err != CborNoError) return err;
	err = cbor_encode_int(datos, dr);
	if (err != CborNoError) return err;






	return err;  // listo para crear submapa con datos después
}


CborError  cbor_CTI_fin_encabezado_encode(CborEncoder* encoder,CborEncoder* datos)
{
	return cbor_encoder_close_container(encoder, datos);
}




CborError  cbor_CTI_init_encabezado_decode(
		uint8_t *buffer_rx,size_t size,
		CborParser *parser,	CborValue *decoder,	CborValue *map_datos,
		CborTag *tag,
		char* accion,size_t *accion_len,
	    int *id,
		int *dr)
{

	//CborValue it;

	CborError err = cbor_parser_init(buffer_rx, size, 0, parser, decoder);

	// Leer tag
	if (cbor_value_is_tag(decoder)) {
		err |= cbor_value_get_tag(decoder, tag);
		err |= cbor_value_advance(decoder);
	}

	// Entrar al mapa raíz
	err |= cbor_value_enter_container(decoder, map_datos);


	// Leer "AC"
	err |= cbor_value_advance(map_datos); // clave "AC"
	err |= cbor_value_copy_text_string(map_datos, accion, accion_len, map_datos);


	// Leer "ID"
	err |= cbor_value_advance(map_datos); // clave "ID"
	err |= cbor_value_get_int(map_datos, id);
	err |= cbor_value_advance(map_datos);


	// Leer "DR"
	err |= cbor_value_advance(map_datos); // clave "ID"
	err |= cbor_value_get_int(map_datos, dr);
	err |= cbor_value_advance(map_datos);


	if (!cbor_value_at_end(map_datos))
	{
		if (cbor_value_is_text_string(map_datos)) {
			char clave[16];
			size_t clave_len = sizeof(clave);
			CborValue temp = *map_datos;

			if (cbor_value_copy_text_string(&temp, clave, &clave_len, NULL) == CborNoError &&
				strcmp(clave, "datos") == 0) {
				err |= cbor_value_advance(map_datos); // clave "datos"
				// aquí el caller puede hacer enter_container(map_datos) si quiere
			}
		}
	}

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





