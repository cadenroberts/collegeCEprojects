import {createContext, useState} from 'react';
import PropTypes from 'prop-types';
export const Context = createContext();
export const ContextProvider = ({children, window,
  initialMessages=[], initialThreads=[]}) => {
  const [remember, setRemember] = useState(false);
  const [rememberEmail, setRememberEmail] = useState('');
  return (
    <Context.Provider value={{remember, setRemember, rememberEmail,
      setRememberEmail, initialMessages, initialThreads, window}}>
      {children}
    </Context.Provider>
  );
};
ContextProvider.propTypes = {
  window: PropTypes.func,
  children: PropTypes.node.isRequired,
  initialMessages: PropTypes.arrayOf(
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
  initialThreads: PropTypes.arrayOf(
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
