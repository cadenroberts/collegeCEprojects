import SignIn from './SignIn.jsx';
import CreateUser from './CreateUser.jsx'
import Home from './Home.jsx';
import Admin from './Admin.jsx';
import AddDevice from './AddDevice.jsx';
import ProtectedRoute from './ProtectedRoute.jsx';
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import React from 'react';

/**
 * Simple component with no state.
 *
 * @return {object} JSX
 */
function App({ router: Router = BrowserRouter }) {
  return (
      <Router>
        <Routes>
          <Route path="/login" element={<SignIn />} />
          <Route path="/home" element={
            <ProtectedRoute>
              <Home />
            </ProtectedRoute>
          }/>
          <Route path="/create" element={
            <ProtectedRoute>
              <CreateUser/>
            </ProtectedRoute>
          }/>
          <Route path="/admin" element={<ProtectedRoute><Admin /></ProtectedRoute>} />
          <Route path="/device" element={<ProtectedRoute><AddDevice /></ProtectedRoute>} />
	  <Route path="/*" element={<ProtectedRoute><Home /></ProtectedRoute>}/>
        </Routes>
      </Router>
  );
}

export default App;
