SELECT people.name as "Starred in Toy Story" FROM "people"
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE movies.title = "Toy Story"