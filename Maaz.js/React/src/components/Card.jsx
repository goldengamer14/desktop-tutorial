import { useState, useEffect } from 'react';

const Card = ({ title, description, image }) => {
    const [hasLiked, setHasLiked] = useState(false);
    const [count, setCount] = useState(0);
    const likeHeart = ["\u{2764}", "\u{1F5A4}"];

    // Runs everytime `hasLiked` changes (`hasLiked` in dependency array)
    useEffect(() => {
        console.log(`User ${!hasLiked ? "dis" : ""}liked the ${title} Card | count is set to ${count}`);
    }, [hasLiked]);

    // Only runs once while rendering (dependency array is empty)
    useEffect(() => console.log("Rendering Card", title), []);

    return (
        <div className="card" onClick={() => setCount(prevCount => prevCount + 1)}>
            <img src={image} alt={title} className="card-image img" />
            <h2 className="card-title">{title} - {count}</h2>
            <p className="card-description">{description}</p>
            <button
                className="butn"
                onClick={() => setHasLiked(prevHasLiked => !prevHasLiked)}>
                {likeHeart[Number(hasLiked)]}
            </button>
        </div>
    );
}

export default Card;