class GameOfLife {
	constructor(canvas) {
		this.ctx = canvas.getContext('2d');
		this.size = 2;
		this.width = Math.floor(this.ctx.canvas.width / this.size);
		this.height = Math.floor(this.ctx.canvas.height / this.size);
		this.grid = this.genGrid(this.width, this.height);
		this.color = 'green';
		this.start(this.grid);
	}
	genState() {
		return Math.floor(Math.random() * 2);
	}
	genCell(col, x, y) {
		if(col) {
	    	this.ctx.fillStyle = this.color;
		    this.ctx.fillRect(x * this.size, y * this.size, this.size, this.size);
		} else {
		    this.ctx.fillStyle = 'yellow';
		    this.ctx.fillRect(x * this.size, y * this.size, this.size, this.size);
		}
	}
	genGrid(rows, cols) {
		return Array.from(Array(rows).keys()).map(row => {
    		return row = Array.from(Array(cols).fill(false).map(col => col = this.genState() ? true : false ));
		});
	}
	displayGrid(grid) {
		grid.forEach((row, x) => {
            row.forEach((col, y) => {
                this.genCell(col, x, y);
            });
		});
		return grid;
	}
	countNeighbors(grid, x, y) {
		let alives = 0;

        if(typeof grid[x-1] !== 'undefined') {
            if(typeof grid[x-1][y-1] !== 'undefined') {
                alives += grid[x-1][y-1] === true ? 1 : 0;
            } else {
                alives += this.genState();
            }
            if(typeof grid[x-1][y+1] !== 'undefined') {
                alives += grid[x-1][y+1] === true ? 1 : 0;
            } else {
                alives += this.genState();
            }
		    alives += grid[x-1][y] === true ? 1 : 0;
		} else {
		    alives += this.genState();
		}

        if(typeof grid[x][y-1] !== 'undefined') {
		    alives += grid[x][y-1] === true ? 1 : 0;
		} else {
		    alives += this.genState();
		}
		if(typeof grid[x][y+1] !== 'undefined') {
		    alives += grid[x][y+1] === true ? 1 : 0;
		} else {
		    alives += this.genState();
		}
		if(typeof grid[x+1] !== 'undefined') {
            if(typeof grid[x+1][y-1] !== 'undefined') {
                alives += grid[x+1][y-1] === true ? 1 : 0;
            } else {
                alives += this.genState();
            }
            if(typeof grid[x+1][y+1] !== 'undefined') {
                alives += grid[x+1][y+1] === true ? 1 : 0;
            } else {
                alives += this.genState();
            }
		    alives += grid[x+1][y] === true ? 1 : 0;
		} else {
		    alives += this.genState();
		}
		return alives;
	}
	liveOrDie(cell, alives) {
		if(cell === true) {
            if(alives < 2 || alives > 3) {
                cell = false;
            }
		} else {
            if(alives == 3) {
                cell = true;
            }
		}
		return cell;
	}
	nextGrid(grid) {
		return grid.map((row, x) => {
            return row.map((cell, y) => {
                let alives = this.countNeighbors(grid, x, y);
                cell = this.liveOrDie(cell, alives);
                return cell;
            });
		});
	}
	process() {
		return new Promise(resolve => {
    		this.grid = this.displayGrid(this.grid);
    		this.grid = this.nextGrid(this.grid);
    		resolve(this.grid);
		});
	}
	start() {
		setInterval( async() => {
	    	this.process(this.grid);
		}, 1);
    }
}