# Thera App – CSE123B Prototype

Overview
--------
Thera App is a full-stack prototype developed for UCSC CSE123B. It demonstrates an integrated healthcare application that manages doctors, patients, and medical devices while providing an interactive web interface and backend API.

The system includes:
- Frontend: React with Vite
- Backend: Node.js/Express with OpenAPI specification
- Database: PostgreSQL with schema and seed data
- Deployment: Docker and Docker Compose
- Assets: 3D model of a hand for visualization

Project Structure
-----------------
Thera_app/
  api/                OpenAPI spec for backend API
    openapi.yaml
  sql/                PostgreSQL schema and seed data
    schema.sql
    data.sql
  src/                React frontend and backend logic
    App.jsx
    main.jsx
    App.css
    server.js         Express backend entrypoint
    app.js            Backend app configuration
    ProtectedRoute.jsx
    Home.jsx
    SignIn.jsx
    CreateUser.jsx
    AddDevice.jsx
    Admin.jsx
    HandModel.jsx     3D model component
  public/             Static assets and 3D models
    therahand.png
    rigged_hand/
      scene.gltf
      scene.bin
      license.txt
      textures/...
  backend_instr.txt   Backend setup instructions
  docker-compose.yml  Full stack container orchestration
  package.json        Node dependencies
  vite.config.js      Vite frontend config
  PSQL.sh             Database setup helper script
  PortClear.sh        Port cleanup helper
  start.sh            Startup script
  .env                Environment variables
  .eslintrc.json      ESLint configuration

Installation and Setup
----------------------
1. Clone the repository and move into the Thera_app directory.

   git clone <repo-url>
   cd Thera_app/Thera_app

2. Install Node dependencies.

   npm install

3. Set up the PostgreSQL database. Either run the provided helper script:

   ./PSQL.sh

   Or run manually:

   psql -U <user> -d <dbname> -f sql/schema.sql
   psql -U <user> -d <dbname> -f sql/data.sql

4. Configure environment variables in a .env file:

   PORT=3000
   DB_HOST=localhost
   DB_PORT=5432
   DB_USER=youruser
   DB_PASS=yourpass
   DB_NAME=thera

5. Run with Docker if preferred:

   docker-compose up --build

Running the App
---------------
Start backend and frontend for local development:

   npm run dev

Frontend will be available at http://localhost:5173
Backend API will be available at http://localhost:3000

API Reference
-------------
The backend API is defined in api/openapi.yaml. You can load this file in Swagger UI or Postman for interactive documentation.

Core endpoints include:
- POST /users      Create a new user
- POST /devices    Add a medical device
- POST /messages   Send doctor-patient message
- GET /patients    Retrieve patient records
- GET /doctors     Retrieve doctor records

Features
--------
- User authentication with protected routes
- Admin panel for managing users and devices
- React component for interactive 3D hand model
- Dockerized deployment
- SQL schema and seed data for reproducibility

Development Notes
-----------------
Scripts included:
- PSQL.sh        Initialize PostgreSQL schema and data
- PortClear.sh   Kill processes on stuck ports
- start.sh       Startup automation

Linting is configured with ESLint (.eslintrc.json).

License
-------
This project was created as a UCSC CSE123B course prototype.
For educational and demonstration purposes only.
