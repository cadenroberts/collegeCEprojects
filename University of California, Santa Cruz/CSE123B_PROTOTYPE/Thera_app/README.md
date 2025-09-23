# Thera App – CSE123B Prototype

Overview
--------
Thera App is a full-stack prototype developed for UCSC CSE123B. It demonstrates a healthcare application that manages doctors, patients, devices, and messaging while providing an interactive web interface and a backend API.

The system includes:
- Frontend: React with Vite
- Backend: Node.js and Express with OpenAPI specification
- Database: PostgreSQL with schema and seed data
- Deployment: Docker and Docker Compose
- Assets: 3D hand model for visualization

Project Structure
-----------------
Thera_app/

  api/                OpenAPI spec for backend API
  
    openapi.yaml
    
  sql/                PostgreSQL schema and seed data
  
    schema.sql
    data.sql
    
  src/                React frontend and backend logic
  
    server.js         Express backend entrypoint
    app.js            Backend app configuration
    App.jsx, main.jsx, CSS and other components
    ProtectedRoute.jsx, SignIn.jsx, Admin.jsx, CreateUser.jsx, AddDevice.jsx
    HandModel.jsx     3D hand component
    
  public/             Static assets and 3D models
  
    therahand.png
    rigged_hand/...
    
  backend_instr.txt   Notes and database usage instructions
  
  docker-compose.yml  Docker orchestration
  
  package.json        Node dependencies
  
  vite.config.js      Vite frontend config
  
  PSQL.sh             Database setup helper script
  
  PortClear.sh        Port cleanup helper
  
  start.sh            Startup script
  
  .env                Environment variables

Setup and App Running
----------------------
  ./start.sh

License
-------
This project was created as a UCSC CSE123B course prototype.
For educational and demonstration purposes only.
