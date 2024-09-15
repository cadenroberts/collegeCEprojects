--
-- DO NOT MODIFY THIS FILE
--

DROP TABLE IF EXISTS mailbox CASCADE;
CREATE TABLE mailbox(id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(), data jsonb);

DROP TABLE IF EXISTS mail;
CREATE TABLE mail(id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(),  mailbox UUID REFERENCES mailbox(id), data jsonb);
