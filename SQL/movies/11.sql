SELECT title as "Title"
FROM "movies"
    JOIN "stars"
    JOIN "ratings"
    JOIN "people" ON (
         movies.id = stars.movie_id
         AND movies.id = ratings.movie_id
         AND stars.person_id = people.id
    )
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC
LIMIT 5