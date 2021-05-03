SELECT title AS "Title", year AS "Release Year"
FROM "movies"
WHERE title LIKE "Harry Potter%"
ORDER BY year ASC