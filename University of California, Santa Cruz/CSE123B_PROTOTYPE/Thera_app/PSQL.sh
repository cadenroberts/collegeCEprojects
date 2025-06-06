#!/bin/bash
docker exec -it $(docker ps -aq) psql -U postgres
