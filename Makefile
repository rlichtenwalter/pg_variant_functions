MODULES = pg_variant_functions
EXTENSION = pg_variant_functions
DATA = pg_variant_functions--1.0.sql
DOCS = README.pg_variant_functions

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
