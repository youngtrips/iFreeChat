create table config (
		id unsigned int auto_increment,
		key varchar(8) not null,
		value varchar(128) not null,
		primary key(id)
);

create table pchat_history (
		id unsigned int auto_increment,
		remote varchar(64) not null,
		diretion unsigned int(1) not null,
		create_time datetime not null,
		message blob not null,
		primary key(id)
);	

create table gchat_history (
		id unsigned int auto_increment,
		group_id unsigned int not null,
		username varchar(64) not null,
		create_time datetime not null,
		message blob not null,
		primary key(id)
);
