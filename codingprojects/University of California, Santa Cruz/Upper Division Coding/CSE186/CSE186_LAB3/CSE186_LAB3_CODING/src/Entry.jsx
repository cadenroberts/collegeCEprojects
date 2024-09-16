import React from 'react';
import PropTypes from 'prop-types';
import './Entry.css';

/**
 * Date entry component.
 */
class Entry extends React.Component {
  /**
   * Constructor
   * @param {object} props - The component properties.
   */
  constructor(props) {
    super(props);
    this.state = {
      inputDate: '',
      validDate: false,
    };
  }

  /**
   * Renders the entry component.
   * @return {object} The rendered component.
   */
  render() {
    return (
      <div className="entryContainer">
        <input
          type="date"
          value={this.state.inputDate}
          onChange={(event) => {
            const [year, month, day] =
                event.target.value.split('-').map(Number);
            this.setState({inputDate: event.target.value,
              validDate: !isNaN((new Date(year, month - 1, day)).getTime())});
          }}
          aria-label="Enter date as MM/DD/YYYY"
        />
        <button
          onClick={() => {
            const [year, month, day] =
                this.state.inputDate.split('-').map(Number);
            if (!isNaN((new Date(year, month - 1, day)).getTime())) {
              this.props.pickerRef.current.setState({
                currentDate: new Date(year, month - 1, day),
                displayDate: new Date(year, month - 1, day)});
            }
          }}
          aria-label="setDate"
          disabled={!this.state.validDate}
        >
          Set
        </button>
      </div>
    );
  }
}

/*
#######################################################################
#######               DO NOT MODIFY BELOW HERE              ###########
#######################################################################
*/

Entry.propTypes = {
  pickerRef: PropTypes.any,
};

export default Entry;
