import React from 'react';
import Radio from '@material-ui/core/Radio';
import RadioGroup from '@material-ui/core/RadioGroup';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import FormControl from '@material-ui/core/FormControl';

export default function Menu(props) {
  const handleChange = (event) => {
    props.onChange(event.target.value);
  };

  const formControls = [];
  for(const [key, value] of Object.entries(props.videos)) {
    formControls.push(<FormControlLabel key={key} value={value[0]} control={<Radio mb={5} />} label={value[1]} />)
  }

  return (
    <FormControl component="fieldset">
      <RadioGroup aria-label="src" name="src" value={props.src} onChange={handleChange}>
          {formControls}
      </RadioGroup>
    </FormControl>
  );
}