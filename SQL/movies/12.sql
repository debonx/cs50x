SELECT a.title AS Title FROM (
    SELECT * FROM "movies"
    JOIN "stars" ON movies.id = stars.movie_id
    JOIN "people" ON stars.person_id = people.id
    WHERE people.name = "Helena Bonham Carter"
) AS a
JOIN "stars" ON a.id = stars.movie_id
JOIN "people" ON stars.person_id = people.id
WHERE people.name = "Johnny Depp"