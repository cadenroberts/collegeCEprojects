import {createContext, useState, useEffect} from 'react';
import PropTypes from 'prop-types';
export const EmailContext = createContext();
export const EmailProvider = ({children, initial=[]}) => {
  const [emails, setEmails] = useState(initial);
  const [selectedMailbox, setSelectedMailbox] = useState('inbox');
  const fetchEmails = async (mailbox) => {
    const response = await fetch(
        `http://localhost:3010/v0/mail?mailbox=${mailbox}`);
    const data = await response.json();
    setEmails(data);
  };
  const handleTrashMove = async (email) => {
    await fetch(
        `http://localhost:3010/v0/mail/${email.id}?mailbox=trash`,
        {method: 'PUT'});
    fetchEmails(selectedMailbox);
  };
  useEffect(() => {
    fetchEmails(selectedMailbox);
  }, [selectedMailbox]);
  return (
    <EmailContext.Provider value={{emails, setEmails, selectedMailbox,
      setSelectedMailbox, handleTrashMove, fetchEmails, initial}}>
      {children}
    </EmailContext.Provider>
  );
};
EmailProvider.propTypes = {
  window: PropTypes.func,
  children: PropTypes.node.isRequired,
  initial: PropTypes.arrayOf(
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
