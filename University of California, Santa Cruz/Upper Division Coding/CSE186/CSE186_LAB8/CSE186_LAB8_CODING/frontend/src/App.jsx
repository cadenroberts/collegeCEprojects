/*
 * Copyright (C) 2022-2024 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without
 * the express written permission of the copyright holder.
 */
import SignIn from './SignIn.jsx';
import Home from './Home.jsx';
import Unauthorized from './Unauthorized.jsx';
import ProtectedRoute from './ProtectedRoute.jsx';
import {ContextProvider} from './Context.jsx';
import {BrowserRouter, Route, Routes} from 'react-router-dom';
import PropTypes from 'prop-types';

/**
 * Simple component with no state.
 *
 * @return {object} JSX
 */
function App({router: Router = BrowserRouter, initialEntries = ['/'],
  initialM = [], initialT = []}) {
  return (
    <ContextProvider initialMessages={initialM} initialThreads={initialT}>
      <Router initialEntries={initialEntries}>
        <Routes>
          <Route path="/" exact element={<SignIn />}/>
          <Route path="/home" element={
            <ProtectedRoute>
              <Home />
            </ProtectedRoute>
          }/>
          <Route path='/unauthorized' element={<Unauthorized />}/>
        </Routes>
      </Router>
    </ContextProvider>
  );
}
App.propTypes = {
  window: PropTypes.func,
  router: PropTypes.elementType,
  initialEntries: PropTypes.arrayOf(PropTypes.string),
  initialM: PropTypes.arrayOf(
      PropTypes.shape({
        id: PropTypes.string.isRequired,
        channel_id: PropTypes.string.isRequired,
        data: PropTypes.shape({
          from: PropTypes.string.isRequired,
          date: PropTypes.string.isRequired,
          content: PropTypes.string.isRequired,
        }).isRequired,
      }),
  ),
  initialT: PropTypes.arrayOf(
      PropTypes.shape({
        id: PropTypes.string.isRequired,
        message_id: PropTypes.string.isRequired,
        data: PropTypes.shape({
          from: PropTypes.string.isRequired,
          date: PropTypes.string.isRequired,
          content: PropTypes.string.isRequired,
        }).isRequired,
      }),
  ),
};

export default App;
