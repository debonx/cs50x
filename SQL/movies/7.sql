SELECT title AS "Title", rating as "Rating"
FROM "movies"
    JOIN "ratings" ON movies.id = ratings.movie_id
WHERE movies.year = 2010
ORDER BY ratings.rating DESC, movies.title ASC