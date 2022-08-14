-- Projetado por: 	Luan Daniel de Oliveira Melo
-- 					(luandanielmelo@gmail.com)
-- Criado em agosto de 2022.
-- 
-- Modelo físico gerado a partir de Modelo Lógico
-- usando freeware brModelo (2.0.0)
-- 
-- Sql ANSI 2003 - brModelo.

CREATE TABLE item_pessoa (
item_id integer,
pessoa_id integer,
PRIMARY KEY(item_id,pessoa_id)
);

CREATE TABLE item (
item_id integer PRIMARY KEY,
nome tinytext,
descr text,
progresso integer,
lista_id integer
);

CREATE TABLE lista (
lista_id integer PRIMARY KEY,
nome tinytext
);

CREATE TABLE pessoa (
pessoa_id integer PRIMARY KEY,
nome tinytext,
email varchar(321)
);

ALTER TABLE item_pessoa ADD FOREIGN KEY(item_id) REFERENCES item (item_id);
ALTER TABLE item_pessoa ADD FOREIGN KEY(pessoa_id) REFERENCES pessoa (pessoa_id);
ALTER TABLE item ADD FOREIGN KEY(lista_id) REFERENCES lista (lista_id);
