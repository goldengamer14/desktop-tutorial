import viteSVG from '/vite.svg';
import reactSVG from './assets/react.svg';
import Card from './components/Card';

const App = () => {
  return (
    <div id="container">
      <h1 id="heading">The Heading</h1>
      <div id="imgs">
        <img className="img" src={viteSVG} alt="Placeholder Image" />
        <img className="img" src={reactSVG} alt="React Logo" />
      </div>

      <Card title="Vite" description="The Vite logo" image={viteSVG} />
      <Card title="React" description="The React logo" image={reactSVG} />
    </div>
  );
}

export default App;