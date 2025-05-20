create table user(email text primary key, display_name text, password text);
insert into user(email, display_name) VALUES ('sidequest_root@hs-aalen.de', 'Sidequest Root User', '');


CREATE TABLE quest (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	caption TEXT NOT NULL,
	parent_id INTEGER,
	FOREIGN KEY (parent_id) REFERENCES quest(id)
);
