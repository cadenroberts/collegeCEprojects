-- Your data insert statements go here
DELETE FROM users;

INSERT INTO users(data) VALUES ('{"email": "molly@books.com","password": "$2b$10$Y00XOZD/f5gBSpDusPUgU.iJufk6Nxx6gAoHRG8t2eHyGgoP2bK4y","name": "Molly Member"}');
INSERT INTO users(data) VALUES ('{"email": "anna@books.com","password": "$2b$10$Y00XOZD/f5gBSpDusPUgU.G1ohpR3oQbbBHK4KzX7dU219Pv/lzze","name": "Anna Admin"}');
INSERT INTO users(data) VALUES ('{"email": "hello@gmail.com","password": "$2b$10$jlxbNK.iahL/ZSPBEevShOkps4ZfCm/83/.J3gIfIbC/9gZOM7pIK","name": "Hello Harry"}');
INSERT INTO users(data) VALUES ('{"email": "a@abc.com","password": "$2b$10$M11TSOnb7LQGSAElX7HAo.g/upzTbAp5U5A9hUMK9KTxj07JUzzau","name": "User A"}');

DELETE FROM workspaces;

INSERT INTO workspaces(owner_id, data) VALUES ((SELECT id FROM users WHERE data->>'email' = 'molly@books.com'), '{"name": "Molly Workspace"}');
INSERT INTO workspaces(owner_id, data) VALUES ((SELECT id FROM users WHERE data->>'email' = 'anna@books.com'), '{"name": "Anna Workspace"}');
INSERT INTO workspaces(owner_id, data) VALUES ((SELECT id FROM users WHERE data->>'email' = 'hello@gmail.com'), '{"name": "Hello Workspace"}');
INSERT INTO workspaces(owner_id, data) VALUES ((SELECT id FROM users WHERE data->>'email' = 'a@abc.com'), '{"name": "User A Workspace"}');

DELETE FROM user_workspaces;

INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'anna@books.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'Molly Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'a@abc.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'Molly Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'a@abc.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'Anna Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'hello@gmail.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'Anna Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'molly@books.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'Hello Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'anna@books.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'Hello Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'hello@gmail.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'User A Workspace'));
INSERT INTO user_workspaces (user_id, workspace_id) VALUES ((SELECT id FROM users WHERE data->>'email' = 'molly@books.com'), (SELECT id FROM workspaces WHERE data->>'name' = 'User A Workspace'));

DELETE FROM channels;

INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'Molly Workspace'), '{"name": "Molly Books"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'Molly Workspace'), '{"name": "Molly Magazines"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'Anna Workspace'), '{"name": "Anna Art"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'Anna Workspace'), '{"name": "Anna Novels"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'Hello Workspace'), '{"name": "Hello IRL"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'Hello Workspace'), '{"name": "Hello Online"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'User A Workspace'), '{"name": "CSE 186"}');
INSERT INTO channels(workspace_id, data) VALUES ((SELECT id FROM workspaces WHERE data->>'name' = 'User A Workspace'), '{"name": "CSE 100"}');

DELETE FROM messages;

INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Molly Books'), '{"from":"Molly Member","content":"Hey, does anyone love Harry Potter too?","date":"2024-06-14T14:16:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Molly Books'), '{"from":"Anna Admin","content":"My favorite book is Where the Red Fern Grows.","date":"2024-06-13T14:16:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Molly Books'), '{"from":"User A","content":"I miss reading.","date":"2024-06-13T14:15:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Molly Magazines'), '{"from":"Molly Member","content":"Has anyone seen the new National Geographic Issue?","date":"2024-06-11T16:14:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Molly Magazines'), '{"from":"Anna Admin","content":"I can not stop buying magazines when I check out at Target.","date":"2024-06-12T14:20:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Molly Magazines'), '{"from":"User A","content":"I have not read Time magazine in a while...","date":"2024-06-13T10:10:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Anna Art'), '{"from":"Anna Admin","content":"I just LOVE the Mona Lisa!","date":"2024-06-09T10:12:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Anna Art'), '{"from":"Hello Harry","content":"Girl with a Pearl Earning always piques my interest.","date":"2024-06-09T14:11:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Anna Art'), '{"from":"User A","content":"The Starry Night is so beautiful.","date":"2024-06-08T17:05:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Anna Novels'), '{"from":"Anna Admin","content":"50 Shades of Grey is overrated.","date":"2024-06-29T17:05:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Anna Novels'), '{"from":"Hello Harry","content":"Goodnight Moon should be considered a novel - the storytelling is incredible.","date":"2024-06-29T14:03:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Anna Novels'), '{"from":"User A","content":"I love the sci-fi genre!","date":"2024-06-28T10:27:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Hello IRL'), '{"from":"Hello Harry","content":"I said hello to professor today!","date":"2024-06-18T15:20:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Hello IRL'), '{"from":"Molly Member","content":"I drove yesterday to see my mom and say hello.","date":"2024-06-18T12:30:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Hello IRL'), '{"from":"Anna Admin","content":"Only my dog seems to say hello to me.","date":"2024-06-17T17:15:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Hello Online'), '{"from":"Hello Harry","content":"I texted that girl I like.","date":"2024-06-12T13:15:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Hello Online'), '{"from":"Molly Member","content":"I posted to Instagram and got 60 likes.","date":"2024-06-12T12:10:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'Hello Online'), '{"from":"Anna Admin","content":"I deleted snapchat today.","date":"2024-06-12T11:05:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'CSE 186'), '{"from":"User A","content":"Gosh, Assignment 8 is taking a long time.","date":"2024-07-23T14:45:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'CSE 186'), '{"from":"Hello Harry","content":"Assignment 1 was by far the hardest!!!","date":"2024-07-21T15:14:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'CSE 186'), '{"from":"Molly Member","content":"Guys PLEASE do the SETS!","date":"2024-07-20T12:15:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'CSE 100'), '{"from":"User A","content":"The lecture made sense but when he got into too much verilog I was confused.","date":"2024-07-15T13:55:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'CSE 100'), '{"from":"Hello Harry","content":"Did anyone finish HW2?","date":"2024-07-14T11:25:19Z"}');
INSERT INTO messages(channel_id, data) VALUES ((SELECT id FROM channels WHERE data->>'name' = 'CSE 100'), '{"from":"Molly Member","content":"Wow, Lab 3 is much harder than 1 and 2!","date":"2024-07-14T09:05:19Z"}');

DELETE FROM threads;

INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Hey, does anyone love Harry Potter too?'), '{"from":"Anna Admin","content":"The Chamber of Secrets is my favorite!","date":"2024-06-14T14:25:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Hey, does anyone love Harry Potter too?'), '{"from":"User A","content":"Yes, though I do like the movies more than the books, sue me!","date":"2024-06-14T14:35:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I can not stop buying magazines when I check out at Target.'), '{"from":"Molly Member","content":"Right? It is like there are staring at me!","date":"2024-06-12T15:20:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I can not stop buying magazines when I check out at Target.'), '{"from":"User A","content":"Try self checkout, if you have to do the checking you will be more focused.","date":"2024-06-12T15:30:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I just LOVE the Mona Lisa!'), '{"from":"User A","content":"Me too!","date":"2024-06-09T10:15:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I just LOVE the Mona Lisa!'), '{"from":"Hello Harry","content":"It feels like she is totally looking at me.","date":"2024-06-09T10:20:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I love the sci-fi genre!'), '{"from":"Anna Admin","content":"The Hunger Games is awesome.","date":"2024-06-28T11:30:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I love the sci-fi genre!'), '{"from":"Hello Harry","content":"The Maze Runner is one of my favorites.","date":"2024-06-28T12:35:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Only my dog seems to say hello to me.'), '{"from":"Hello Harry","content":"Dogs are great companions. I have a golden retriever.","date":"2024-06-17T18:20:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Only my dog seems to say hello to me.'), '{"from":"Molly Member","content":"I have a cat. She will not say hello.","date":"2024-06-17T18:30:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I texted that girl I like.'), '{"from":"Anna Admin","content":"Good for you Harry!","date":"2024-06-12T13:20:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'I texted that girl I like.'), '{"from":"Molly Member","content":"Hey Harry sorry to reply here... but I just got a text from you?","date":"2024-06-12T13:25:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Gosh, Assignment 8 is taking a long time.'), '{"from":"Molly Member","content":"DO NOT FORGET COVERAGE AND TESTING!!!","date":"2024-07-23T14:46:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Gosh, Assignment 8 is taking a long time.'), '{"from":"Hello Harry","content":"I developed only my front end first and now I am totally stuck.","date":"2024-07-23T14:50:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Wow, Lab 3 is much harder than 1 and 2!'), '{"from":"User A","content":"Check that you are not using async and synchronous clock timings!","date":"2024-07-14T10:05:19Z"}');
INSERT INTO threads(message_id, data) VALUES ((SELECT id FROM messages WHERE data->>'content' = 'Wow, Lab 3 is much harder than 1 and 2!'), '{"from":"Hello Harry","content":"Try going to office hours, it helped me.","date":"2024-07-14T11:15:19Z"}');


-- Each SQL statement must be on a single line