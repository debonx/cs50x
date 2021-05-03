import React from 'react';

export class Video extends React.Component {
  render() {
    return (
      <div>
        <iframe width="100%" height="500" src={this.props.src} />
      </div>
    );
  }
}