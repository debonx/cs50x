SELECT DISTINCT people.name as "Directed Movie above rating 9.0"
FROM "people"
    INNER JOIN directors ON people.id = directors.person_id
    INNER JOIN movies ON directors.movie_id = movies.id
    INNER JOIN ratings ON movies.id = ratings.movie_id
WHERE ratings.rating >= 9.0