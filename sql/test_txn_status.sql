CREATE EXTENSION pg_txn_status;

/* Valid values */
SELECT 'begin'::txn_status;
SELECT 'prepare'::txn_status;
SELECT 'commit'::txn_status;
SELECT 'rollback'::txn_status;
SELECT 'complete'::txn_status;
SELECT 'incomplete'::txn_status;
SELECT NULL::txn_status;

/* Invalid values */
SELECT ''::txn_status;
SELECT 'start'::txn_status;
SELECT 'completed'::txn_status;

/* Storing values to the table */
CREATE TABLE test(id serial, status txn_status);
INSERT INTO test (status)
VALUES
	('begin'),
	('prepare'),
	('commit'),
	('rollback'),
	('complete'),
	('incomplete');
SELECT * FROM test;

/* Equality operator */
SELECT 'begin'::txn_status = 'begin'::txn_status;
SELECT 'begin'::txn_status = 'commit'::txn_status;
SELECT * FROM test WHERE status = 'rollback';