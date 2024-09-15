-- Your DDL statements go here
DROP TABLE IF EXISTS users CASCADE;
CREATE TABLE users(id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(),  data jsonb);

DROP TABLE IF EXISTS workspaces CASCADE;
CREATE TABLE workspaces (id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(), owner_id UUID REFERENCES users(id), data JSONB);
-- JOIN TABLE allowed to see workspace but doesn't own it 2 columns foreign key to users/workspaces

DROP TABLE IF EXISTS user_workspaces CASCADE;
CREATE TABLE user_workspaces (user_id UUID REFERENCES users(id), workspace_id UUID REFERENCES workspaces(id), PRIMARY KEY (user_id, workspace_id));

DROP TABLE IF EXISTS channels CASCADE;
CREATE TABLE channels (id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(), workspace_id UUID REFERENCES workspaces(id), data JSONB);

-- messages should have id reference to user NOT jsonb 'from'
DROP TABLE IF EXISTS messages CASCADE;
CREATE TABLE messages (id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(), channel_id UUID REFERENCES channels(id), data JSONB);

DROP TABLE IF EXISTS threads;
CREATE TABLE threads (id UUID UNIQUE PRIMARY KEY DEFAULT gen_random_uuid(), message_id UUID REFERENCES messages(id), data JSONB);
-- Each DDL statement must be on a single line