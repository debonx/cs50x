SELECT DISTINCT(a.name) FROM (
    SELECT * FROM people AS b WHERE NOT b.name = "Kevin Bacon"
    UNION
    SELECT * FROM people AS c WHERE c.name = "Kevin Bacon" AND NOT birth = 1958
) AS a
JOIN "stars" ON a.id = stars.person_id
JOIN "movies" ON stars.movie_id = movies.id
WHERE movies.id IN (
    SELECT d.id FROM movies AS d
    JOIN "stars" ON d.id = stars.movie_id
    JOIN "people" ON stars.person_id = people.id
    WHERE people.name = "Kevin Bacon" AND people.birth = 1958
)
ORDER BY a.name ASC