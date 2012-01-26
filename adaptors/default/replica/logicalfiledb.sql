-- Database  : logicalfiledb
-- Version   : PostgreSQL 8.2.3 on i686-pc-mingw32, compiled by GCC gcc.exe (GCC) 3.4.2 (mingw-special)

-- SET client_encoding = 'UTF8';
-- SET standard_conforming_strings = off;
-- SET check_function_bodies = false;
-- SET client_min_messages = warning;
-- SET escape_string_warning = off;

--
-- Create database
--
-- CREATE DATABASE "logicalfiledb"
--  WITH 
--    OWNER = "postgres"
--    TEMPLATE = template0
--    ENCODING = 'UTF8'
--    TABLESPACE = "pg_default";

-- \connect logicalfiledb

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;
SET search_path = public, pg_catalog;
SET default_tablespace = '';
SET default_with_oids = false;

--
-- Create generic user: SAGA, password: SAGA_client
--
CREATE ROLE "SAGA";
ALTER ROLE "SAGA" 
  WITH 
    NOSUPERUSER 
    INHERIT 
    NOCREATEROLE 
    NOCREATEDB 
    LOGIN PASSWORD 'md5d6ef92a5b376ddcc9556f2b201dcc01c'
      VALID UNTIL 'infinity';

--
-- Create query languages
--
CREATE TRUSTED PROCEDURAL LANGUAGE "plpgsql"
  HANDLER "plpgsql_call_handler"
  VALIDATOR "plpgsql_validator";
SET check_function_bodies = false;

CREATE PROCEDURAL LANGUAGE "plpythonu"
  HANDLER "plpython_call_handler";
  
--
-- Structure for table data: 
--
CREATE TABLE data (
    node_id integer NOT NULL,
    filename character varying NOT NULL
);

ALTER TABLE public.data OWNER TO postgres;

--
-- Structure for table main: 
--
CREATE TABLE main (
    id integer NOT NULL,
    "key" character varying(32) NOT NULL,
    value character varying(128)
);

ALTER TABLE public.main OWNER TO postgres;

--
-- Definition for sequence main_id_seq: 
--
CREATE SEQUENCE main_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;

ALTER TABLE public.main_id_seq OWNER TO postgres;
ALTER SEQUENCE public.main_id_seq OWNED BY public.main.id;
ALTER TABLE public.main ALTER COLUMN id 
  SET DEFAULT nextval('main_id_seq'::regclass);

--
-- Structure for table nodes: 
--
CREATE TABLE nodes (
    node_id integer NOT NULL,
    node_name character varying NOT NULL,
    parent_id integer NOT NULL,
    is_directory integer DEFAULT 0 NOT NULL
);

ALTER TABLE public.nodes OWNER TO postgres;

--
-- Definition for sequence nodes_node_id_seq: 
--
CREATE SEQUENCE nodes_node_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;

ALTER TABLE public.nodes_node_id_seq OWNER TO postgres;
ALTER SEQUENCE public.nodes_node_id_seq OWNED BY public.nodes.node_id;
ALTER TABLE public.nodes ALTER COLUMN node_id 
  SET DEFAULT nextval('nodes_node_id_seq'::regclass);

--
-- Structure for table metadata: 
--
CREATE TABLE metadata (
    node_id integer NOT NULL,
    metakey character varying NOT NULL,
    metavalue character varying NOT NULL,
    is_vector integer DEFAULT 0 NOT NULL
);

ALTER TABLE public.metadata OWNER TO postgres;

--
-- Definition for function match: 
--
SET search_path = public, pg_catalog;
CREATE FUNCTION "match" (character varying, character varying) RETURNS boolean
    AS $$
    import re
    if (SD.has_key(args[0]) == False):
        SD[args[0]] = re.compile(args[0])
    return SD[args[0]].match(args[1]) != None
$$
    LANGUAGE plpythonu STABLE;

ALTER FUNCTION public."match"(character varying, character varying) OWNER TO postgres;

--
-- Definition for function insert_nodes_trigger_proc: 
--
CREATE FUNCTION insert_nodes_trigger_proc () RETURNS "trigger"
    AS $$
BEGIN
INSERT INTO metadata(node_id, metakey, metavalue, is_vector)
    VALUES(new.node_id, 'ctime', CURRENT_TIMESTAMP, 0);
INSERT INTO metadata(node_id, metakey, metavalue, is_vector)
    VALUES(new.node_id, 'utime', CURRENT_TIMESTAMP, 0);
UPDATE metadata set metavalue = CURRENT_TIMESTAMP
    WHERE new.parent_id = node_id AND metakey = 'utime';
RETURN NEW;
END;
$$
    LANGUAGE plpgsql;

ALTER FUNCTION public.insert_nodes_trigger_proc() OWNER TO postgres;

--
-- Definition for function insert_update_data_trigger_proc: 
--
CREATE FUNCTION insert_update_data_trigger_proc () RETURNS "trigger"
    AS $$
BEGIN
UPDATE metadata set metavalue = CURRENT_TIMESTAMP
    WHERE new.node_id = node_id AND metakey = 'utime';
RETURN NEW;
END;
$$
    LANGUAGE plpgsql;

ALTER FUNCTION public.insert_update_data_trigger_proc() OWNER TO postgres;

--
-- Definition for function delete_nodes_trigger_proc: 
--
CREATE FUNCTION delete_nodes_trigger_proc () RETURNS "trigger"
    AS $$
BEGIN
DELETE FROM metadata WHERE OLD.node_id = node_id;
UPDATE metadata set metavalue = CURRENT_TIMESTAMP
    WHERE OLD.parent_id = node_id AND metakey = 'utime';
RETURN OLD;
END;
$$
    LANGUAGE plpgsql;

ALTER FUNCTION public.delete_nodes_trigger_proc() OWNER TO postgres;

--
-- Definition for index metadata_node_id_idx: 
--
CREATE INDEX metadata_node_id_idx ON public.metadata USING btree (node_id);

--
-- Definition for index metadata_metakey_idx: 
--
CREATE INDEX metadata_metakey_idx ON public.metadata USING btree (metakey);

--
-- Definition for index data_pkey: 
--
ALTER TABLE ONLY data
    ADD CONSTRAINT data_pkey PRIMARY KEY (node_id);

--
-- Definition for index main_pkey: 
--
ALTER TABLE ONLY main
    ADD CONSTRAINT main_pkey PRIMARY KEY (id);

--
-- Definition for index main_key_key: 
--
ALTER TABLE ONLY main
    ADD CONSTRAINT main_key_key UNIQUE ("key");

--
-- Definition for index nodes_pkey: 
--
ALTER TABLE ONLY nodes
    ADD CONSTRAINT nodes_pkey PRIMARY KEY (node_id);

--
-- Definition for trigger insert_nodes: 
--
CREATE TRIGGER insert_nodes
    AFTER INSERT ON nodes
    FOR EACH ROW
    EXECUTE PROCEDURE insert_nodes_trigger_proc ();
--
-- Definition for trigger insert_update_data: 
--
CREATE TRIGGER insert_update_data
    AFTER INSERT OR UPDATE ON data
    FOR EACH ROW
    EXECUTE PROCEDURE insert_update_data_trigger_proc ();
--
-- Definition for trigger delete_nodes: 
--
CREATE TRIGGER delete_nodes
    BEFORE DELETE ON nodes
    FOR EACH ROW
    EXECUTE PROCEDURE delete_nodes_trigger_proc ();

--
-- Comments
--
COMMENT ON SCHEMA public IS 'Standard public schema';

--
-- Grant permissions
--
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON LANGUAGE plpgsql TO postgres;
GRANT ALL ON FUNCTION delete_nodes_trigger_proc() TO postgres;
GRANT ALL ON FUNCTION insert_nodes_trigger_proc() TO postgres;
GRANT ALL ON FUNCTION insert_update_data_trigger_proc() TO postgres;
GRANT ALL ON FUNCTION "match"(character varying, character varying) TO postgres;
GRANT ALL ON TABLE main TO postgres;
GRANT ALL ON TABLE data TO postgres;
GRANT ALL ON TABLE metadata TO postgres;
GRANT ALL ON TABLE nodes TO postgres;
GRANT ALL ON SEQUENCE main_id_seq TO postgres;
GRANT ALL ON SEQUENCE nodes_node_id_seq TO postgres;

GRANT USAGE ON SCHEMA public TO "SAGA";
GRANT ALL ON FUNCTION delete_nodes_trigger_proc() TO "SAGA";
GRANT ALL ON FUNCTION insert_nodes_trigger_proc() TO "SAGA";
GRANT ALL ON FUNCTION insert_update_data_trigger_proc() TO "SAGA";
GRANT ALL ON FUNCTION "match"(character varying, character varying) TO "SAGA";
GRANT ALL ON TABLE main TO "SAGA";
GRANT ALL ON TABLE data TO "SAGA";
GRANT ALL ON TABLE metadata TO "SAGA";
GRANT ALL ON TABLE nodes TO "SAGA";
GRANT SELECT,UPDATE ON SEQUENCE main_id_seq TO "SAGA";
GRANT SELECT,UPDATE ON SEQUENCE nodes_node_id_seq TO "SAGA";

--
-- Data for table main
--
INSERT INTO main (id, "key", value) VALUES (1, 'version', '256');
INSERT INTO main (id, "key", value) VALUES (2, 'type', 'logicalfile');

--
-- Data for table nodes
--
INSERT INTO nodes (node_id, node_name, parent_id, is_directory) VALUES (1, '__root__', 0, 1);
