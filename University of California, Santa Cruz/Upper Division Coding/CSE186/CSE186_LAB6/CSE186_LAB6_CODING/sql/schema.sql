DROP TABLE IF EXISTS mailbox CASCADE;
CREATE TABLE mailbox(id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(), name VARCHAR(32));

DROP TABLE IF EXISTS mail;
CREATE TABLE mail(id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(),  mailbox UUID REFERENCES mailbox(id), mail jsonb);
