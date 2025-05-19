/*
 * 
 */

//////////////////////////////////////////////

#include "cbor_CTI.h"

/*DEFINE***************************************************************************************/


/*STRUCT***************************************************************************************/


/*VARIABLES*********************************************************************************************/ 

struct {
	CborEncoder *encoder;
	CborEncoder *map;
}VA_cbor_CTI;



/*FUNCIONES**********************************************************************************/


/**********************************************************************************************/


CborError  cbor_CTI_init_encabezado_encode(
		uint8_t *buffer_montar, size_t size,
		CborEncoder* encoder,
	    CborEncoder* out_map,
	    uint64_t tag,
		const char* accion,
	    int id)
{
	CborError err;

	VA_cbor_CTI.encoder=encoder;
	VA_cbor_CTI.map=out_map;
	cbor_encoder_init(encoder, buffer_montar, size, 0);

	 // Codificar tag
	err = cbor_encode_tag(encoder, tag);
	if (err != CborNoError) return err;

	// Crear mapa raíz con 3 entradas ("accion", "ID", "datos")
	err = cbor_encoder_create_map(encoder, out_map, 3);  // 3 claves esperadas
	if (err != CborNoError) return err;

	 // "accion": "R" (por ejemplo)
	err = cbor_encode_text_stringz(out_map, "AC");
	if (err != CborNoError) return err;
	err = cbor_encode_text_stringz(out_map, accion);
	if (err != CborNoError) return err;


	// "ID": 42 (por ejemplo)
	err = cbor_encode_text_stringz(out_map, "ID");
	if (err != CborNoError) return err;
	err = cbor_encode_int(out_map, id);
	if (err != CborNoError) return err;

	//"datos" → nombre clave, pero no se cierra el mapa interior aquí
	err = cbor_encode_text_stringz(out_map, "datos");



	return err;  // listo para crear submapa con datos después
}


CborError  cbor_CTI_fin_encabezado_encode(void)
{
	return cbor_encoder_close_container(VA_cbor_CTI.encoder, VA_cbor_CTI.map);
}




CborError  cbor_CTI_init_encabezado_decode(
		uint8_t *buffer_rx,size_t size,
		const char* accion,uint8_t accion_len,
	    int *id)
{

	CborParser parser;
	CborValue it, map, datos, rel_array, rel_item;
	CborError err = cbor_parser_init(buffer_rx, size, 0, &parser, &it);


	// Leer tag
	if (cbor_value_is_tag(&it)) {
		CborTag tag;
		err |= cbor_value_get_tag(&it, tag);
		err |= cbor_value_advance(&it);
	}


	// Entrar al mapa raíz
	err |= cbor_value_enter_container(&it, &map);


	// Leer "AC"
	err |= cbor_value_advance(&map); // clave "AC"
	err |= cbor_value_copy_text_string(&map, accion, &accion_len, &map);


	// Leer "ID"
	err |= cbor_value_advance(&map); // clave "ID"
	err |= cbor_value_get_int(&map, id);
	err |= cbor_value_advance(&map);




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





