import ResponsiveDrawer from './ResponsiveDrawer.jsx';
import {EmailProvider} from './EmailContext.jsx';
import PropTypes from 'prop-types';

/**
 * Simple component with no state.
 *
 * See the basic-react from lecture for an example of adding and
 * reacting to changes in state.
 *
 * @return {object} JSX
 */
function App({initialEmails=[]}) {
  return (
    <EmailProvider initial={initialEmails}>
      <ResponsiveDrawer />
    </EmailProvider>
  );
}

App.propTypes = {
  initialEmails: PropTypes.arrayOf(
      PropTypes.shape({
        id: PropTypes.string.isRequired,
        to: PropTypes.shape({
          name: PropTypes.string.isRequired,
          email: PropTypes.string.isRequired,
        }).isRequired,
        from: PropTypes.shape({
          name: PropTypes.string.isRequired,
          email: PropTypes.string.isRequired,
        }).isRequired,
        received: PropTypes.string.isRequired,
        sent: PropTypes.string.isRequired,
        subject: PropTypes.string.isRequired,
      }),
  ),
};

export default App;
