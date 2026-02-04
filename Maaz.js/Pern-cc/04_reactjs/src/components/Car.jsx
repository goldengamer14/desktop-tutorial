const Car = ({ carId, name, type, mileage, createdAt }) => {
    return (
        <tr>
            <td>{carId}</td>
            <td>{name}</td>
            <td>{type}</td>
            <td>{mileage}</td>
            <td>{new Date(createdAt).toLocaleString()}</td>
        </tr>
    );
}

const CarHeader = () => {
    return (
            <tr>
                <th>ID</th>
                <th>Name</th>
                <th>Type</th>
                <th>Mileage</th>
                <th>Created At</th>
            </tr>
    );
}

export { Car, CarHeader };