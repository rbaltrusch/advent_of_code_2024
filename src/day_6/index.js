// run with deno inside src/day_6 directory

const text = await Deno.readTextFile("data.txt");

const line_width = text.indexOf("\n");
const starting_position = text.indexOf("^");
const start_y = Math.floor(starting_position / line_width);
const start_x = starting_position - (start_y * (line_width + 1)); // 1 offset due to \n ending each line

const EMPTY = ".";
const OBSTRUCTION = "#";

const directions = [
  [0, -1], // upward
  [1, 0], // to right
  [0, 1], // down
  [-1, 0], // left
];

function determine_visited_cells(lines, current_x, current_y) {
  let current_direction_index = 0;
  let unchanged_counter = 0; // keeps track of how long the length of visited cells has not increased
  const visited_cells = new Set(["initial_pos"]);
  while (true) {
    const [x, y] = directions[current_direction_index];
    const next_line = lines[current_y + y] ?? [];
    const next_cell = next_line[current_x + x];

    // out of bounds means path is finished
    if (!next_cell) break;

    // change direction if next cell is obstructed
    if (next_cell == OBSTRUCTION) {
      current_direction_index++;
      current_direction_index %= directions.length;
      continue;
    }

    // continue walking in direction
    current_y += y;
    current_x += x;
    const visited_cell = `${current_x};${current_y}`;
    if (visited_cells.has(visited_cell)) unchanged_counter++;
    visited_cells.add(visited_cell);

    // running in a loop
    if (unchanged_counter >= visited_cells.size) {
      return { size: visited_cells.size, cyclic: true };
    }
  }
  return { size: visited_cells.size, cyclic: false };
}

// part 1: walk around maze and count visited cells
{
  const result = determine_visited_cells(text.split("\n"), start_x, start_y);
  console.log(result.size);
}

// part 2: count amount of cells at which a new obstruction would cause a recurring cyclical maze walk
String.prototype.replaceAt = function (index, replacement) {
  return this.substring(0, index) + replacement +
    this.substring(index + replacement.length);
};

let cycles = 0;
for (let i = 0; i < text.length; i++) {
  if (text[i] != EMPTY) continue;
  const lines = text.replaceAt(i, OBSTRUCTION).split("\n");
  const res = determine_visited_cells(lines, start_x, start_y);
  if (res.cyclic) cycles++;
}
console.log(cycles);
