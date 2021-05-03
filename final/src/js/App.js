import React from 'react';
import ReactDOM from 'react-dom';
import Container from '@material-ui/core/Container';
import { Video } from './components/Video';
import Menu from './components/Menu';
import SearchAppBar from './components/SearchAppBar';
import Grid from '@material-ui/core/Grid';

class App extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      fetched: false,
      videos: {},
      src: ''
    };
    this.handleClick = this.handleClick.bind(this);
    this.handleChange = this.handleChange.bind(this);
    window.happyvideosapp = this;
  }

  /**
   * Fetch Videos from Google
   */
  fetchVideos() {
    // Start Google Api Client
    gapi.client.init({
      'apiKey': 'AIzaSyD0VejOjeGeyrl3Hdwjy16ryc6t1-mv-Qk',
      'discoveryDocs': ['https://www.googleapis.com/discovery/v1/apis/youtube/v3/rest'],
    })
    .then(() => {
      // Define relevant word to search
      const query = ['pranks', 'funny', 'prank', 'fun', 'jokes'];
      const randomIndex = () => Math.floor(Math.random() * ((query.length - 1) - 0 + 1) + 0);
      const selectedQuery = query[randomIndex()];
      const count = 10;
      // Make the request
      return gapi.client.youtube.search.list({
        "part": [
          "snippet"
        ],
        "maxResults": count,
        "order": "date",
        "q": selectedQuery,
        "relevanceLanguage": "en",
        "type": [
          "video"
        ],
        "videoDuration": "short"
      })
    })
    .then((response) => {
      // Process Videos
      const resObj = JSON.parse(response.body);
      const videos = {};
      const youTubeUrl = 'https://www.youtube.com/embed/';
      resObj.items.forEach(video => {
        videos[video.id.videoId] = [`${youTubeUrl}${video.id.videoId}`, video.snippet.title];
      });

      window.happyvideosapp.setState(state => ({
        fetched: true,
        videos: {
          ...videos
        },
        src: videos[Object.keys(videos)[0]][0]
      }));

    }, reason => {
      console.log('Error: ' + reason.result.error.message);
    });
  };

  /**
   * Load Google API
   */
  loadGapi() {
    gapi.load('client', this.fetchVideos);
  }

  componentDidMount() {
    if (!this.fetched) {
      // Initialize Google API
      this.loadGapi();
    }
  }

  handleClick() {
    this.loadGapi();
  }

  handleChange(newSrc) {
    this.setState(state => ({
      src: newSrc
    }));
  }

  render() {
    return (
      <Container>
          <SearchAppBar onClick={this.handleClick} />
          <h1>The Happy Video Player</h1>
          <p>This is meant to spread some joy around. It fetches some funny videos from YouTube. I hope you enjoy it.</p>
          <Grid container spacing={3}>
            <Grid item xs={4}>
              <Menu onChange={this.handleChange} videos={this.state.videos} />
            </Grid>
            <Grid item xs={8}>
              <Video src={this.state.src} />
            </Grid>
          </Grid>
      </Container>
    );
  }
}

ReactDOM.render(
  <App />,
  document.getElementById('app')
);