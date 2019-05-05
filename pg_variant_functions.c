#include <postgres.h>
#include <utils/lsyscache.h>
#include <utils/array.h>
#include <funcapi.h>
#include <access/htup_details.h>
#include <stddef.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1( alternate_allele_frequency );
Datum alternate_allele_frequency( PG_FUNCTION_ARGS ) {
	ArrayType* values;
	Oid values_type;
	int16 values_width;
	bool values_passbyvalue;
	char values_alignmentcode;
	Datum* values_content;
	bool* values_nullflags;
	int values_length;
	ArrayType* indices;
	Oid indices_type;
	int16 indices_width;
	bool indices_passbyvalue;
	char indices_alignmentcode;
	Datum* indices_content;
	bool* indices_nullflags;
	int indices_length;
	int nonnull_count;
	int alternate_count;
	int i;
	
	values = PG_GETARG_ARRAYTYPE_P( 0 );
	values_type = ARR_ELEMTYPE( values );
	get_typlenbyvalalign( values_type, &values_width, &values_passbyvalue, &values_alignmentcode );
	deconstruct_array( values, values_type, values_width, values_passbyvalue, values_alignmentcode, &values_content, &values_nullflags, &values_length );

	indices = PG_GETARG_ARRAYTYPE_P(1);
	indices_type = ARR_ELEMTYPE( indices );
	get_typlenbyvalalign( indices_type, &indices_width, &indices_passbyvalue, &indices_alignmentcode );
	deconstruct_array( indices, indices_type, indices_width, indices_passbyvalue, indices_alignmentcode, &indices_content, &indices_nullflags, &indices_length );
	nonnull_count = 0;
	alternate_count = 0;
	for( i = 0; i < indices_length; ++i ) {
		if( !indices_nullflags[i] && indices_content[i] - 1 < (long unsigned)values_length ) {
			if( !values_nullflags[ indices_content[i] - 1 ] ) {
				++nonnull_count;
				alternate_count += values_content[ indices_content[i] - 1 ];
			}
		}
	}

	if( nonnull_count == 0 ) {
		PG_RETURN_NULL();
	} else {
		PG_RETURN_FLOAT4( alternate_count/(2.0*nonnull_count) );
	}
}

PG_FUNCTION_INFO_V1( summarize_variant );
Datum summarize_variant( PG_FUNCTION_ARGS ) {
	TupleDesc tuple_desc;
	ArrayType* values
	Oid values_type;
	int16 values_width;
	bool values_passbyvalue;
	char values_alignmentcode;
	Datum* values_content;
	bool* values_nullflags;
	int values_length;
	size_t composite_type_elements
	Datum* results_content;
	bool* results_nullflags;
	ArrayType* indices;
	Oid indices_type;
	int16 indices_width;
	bool indices_passbyvalue;
	char indices_alignmentcode;
	Datum* indices_content;
	bool* indices_nullflags;
	int indices_length;
	int count;
	unsigned int nonnull_count;
	unsigned int alternate_count;
	int i;
	
	if( PG_ARGISNULL(0) ) {
		ereport( ERROR, (errmsg("summarize_variant: array of values must be non-null")) );
	}

	if( get_call_result_type( fcinfo, NULL, &tuple_desc ) != TYPEFUNC_COMPOSITE ) {
		ereport( ERROR, (errmsg("summarize_variant: function returning composite type called in context that cannot accept composite type")) );
	}
	tuple_desc = BlessTupleDesc( tuple_desc );

	values = PG_GETARG_ARRAYTYPE_P( 0 );
	values_type = ARR_ELEMTYPE( values );
	get_typlenbyvalalign( values_type, &values_width, &values_passbyvalue, &values_alignmentcode );
	deconstruct_array( values, values_type, values_width, values_passbyvalue, values_alignmentcode, &values_content, &values_nullflags, &values_length );

	composite_type_elements = 9;
	results_content = (Datum*)palloc0( sizeof(Datum) * composite_type_elements );
	results_nullflags = (bool*)palloc0( sizeof(bool) * composite_type_elements );
	// FORMAT
	// [0] - call rate
	// [1] - alternate allele frequency
	// [2] - first subset call rate
	// [3] - second subset call rate
	// [4] - first subset alternate allele frequency
	// [5] - second subset alternate allele frequency

	if( !PG_ARGISNULL(1) ) {
		indices = PG_GETARG_ARRAYTYPE_P(1);
		indices_type = ARR_ELEMTYPE( indices );
		get_typlenbyvalalign( indices_type, &indices_width, &indices_passbyvalue, &indices_alignmentcode );
		deconstruct_array( indices, indices_type, indices_width, indices_passbyvalue, indices_alignmentcode, &indices_content, &indices_nullflags, &indices_length );
		count = 0;
		nonnull_count = 0;
		alternate_count = 0;
		for( i = 0; i < indices_length; ++i ) {
			if( !indices_nullflags[i] && indices_content[i] - 1 < (long unsigned)values_length ) {
				++count;
				if( !values_nullflags[ indices_content[i] - 1 ] ) {
					++nonnull_count;
					alternate_count += values_content[ indices_content[i] - 1 ];
				}
			}
		}
		results_content[2] = Float4GetDatum( nonnull_count / (float4)count );
		results_content[3] = Float4GetDatum( nonnull_count == 0 ? 0 : alternate_count/(2.0*nonnull_count) );
		results_nullflags[3] = nonnull_count == 0;
	} else {
		results_nullflags[2] = true;
		results_nullflags[3] = true;
	}
	
	if( !PG_ARGISNULL(2) ) {
		indices = PG_GETARG_ARRAYTYPE_P(2);
		indices_type = ARR_ELEMTYPE( indices );
		get_typlenbyvalalign( indices_type, &indices_width, &indices_passbyvalue, &indices_alignmentcode );
		deconstruct_array( indices, indices_type, indices_width, indices_passbyvalue, indices_alignmentcode, &indices_content, &indices_nullflags, &indices_length );
		count = 0;
		nonnull_count = 0;
		alternate_count = 0;
		for( i = 0; i < indices_length; ++i ) {
			if( !indices_nullflags[i] && indices_content[i] - 1 < (long unsigned)values_length ) {
				++count;
				if( !values_nullflags[ indices_content[i] - 1 ] ) {
					++nonnull_count;
					alternate_count += values_content[ indices_content[i] - 1 ];
				}
			}
		}
		results_content[4] = Float4GetDatum( nonnull_count / (float4)count );
		results_content[5] = Float4GetDatum( nonnull_count == 0 ? 0 : alternate_count/(2.0*nonnull_count) );
		results_nullflags[5] = nonnull_count == 0;
	} else {
		results_nullflags[4] = true;
		results_nullflags[5] = true;
	}

	count = values_length;
	nonnull_count = 0;
	alternate_count = 0;
	for( i = 0; i < values_length; ++i ) {
		if( !values_nullflags[i] ) {
			++nonnull_count;
			alternate_count += values_content[i];
		}
	}

	results_content[0] = Float4GetDatum( nonnull_count / (float4)count );
	results_content[1] = Float4GetDatum( nonnull_count == 0 ? 0 : alternate_count/(2.0*nonnull_count) );
	results_nullflags[1] = nonnull_count == 0;

	HeapTuple tuple = heap_form_tuple( tuple_desc, results_content, results_nullflags );
	Datum result = HeapTupleGetDatum( tuple );
	PG_RETURN_DATUM( result );
}
