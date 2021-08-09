# wrld3d-exercise
My solution for finding the most isolated point on a large 2D grid for a job application.

Reads a file from stdin where each line is in the format "{place_name} {x_coord} {y_coord}", puts all of these into a 2D tree, finds the nearest neighbour for each point and finally outputs the name of the most isolated point.

## Time complexity

There are three main steps to analyse:
 * reading and parsing input: O(n),
 * building a kd-tree: O(n log(n)),
 * nearest neighbour search: close to O(log(n)), performed on each element so O(n log(n)) overall.

Time complexity for the latter two is according to the ALGLIB documentation, https://www.alglib.net/other/nearestneighbors.php.

Overall, the time complexity should be O(n log(n)) or very much near it.

## How to build and run 
On linux:

    make most_isolated # to build
    ./most_isolated < problem_big.txt # to run

## Credits
The program uses the ALGLIB data processing library for its kd-tree and nearest neighbour search.

The source code is included in this repository; however, if you do not trust all of the files to be genuine, you can download a copy from https://www.alglib.net/, and then just copy all files from the zip file's /cpp/src/ into this repository's /alglib/.

## Licence 
GPLv3, a copy is provided in /gpl3.txt or https://www.gnu.org/licenses/gpl-3.0.en.html.

