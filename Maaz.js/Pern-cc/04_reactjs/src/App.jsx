import { useState, useEffect } from "react";
import {Car, CarHeader} from "./components/Car";

const App = () => {
  const [cars, setCars] = useState([]);

  useEffect(() => {
    // Fetch cars from the backend API
    fetch("api/v1/cars")
      .then(res => res.json())
      .then(data => {
        setCars(data);
      })
      .catch(err => console.error("Error fetching cars:", err));
  }, []);

  console.log("Fetched cars:", cars);

  return (
    <div id="container">

      <h1 className="header">Welcome to My Cars Showroom</h1>
      <div id="app-div">App</div>

      <table cellSpacing="0" cellPadding="10" border="2" id="cars-table">
        <thead>
          <CarHeader />
        </thead>
        <tbody>
          {Object.keys(cars).map(carId => (
            <Car key={carId} carId={carId} {...cars[carId]} />
          ))}
        </tbody>
      </table>

    </div>
  );
}

export default App;