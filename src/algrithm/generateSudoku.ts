import { sudokuValue, Level } from '../types';

/**
 * 生成数独
 *
 * @param level 表示难度,取值0,1,2,3; 0-随机, 1-3难度递增
 *
 * @returns tuple [generate,answer] generate:元组的第一个元素,9x9 matrix   answer:元组的第二个元素, 9x9 matrix
 *
 *
 */
const a = null;
const b = 0;
let Sudoku: Array<Array<sudokuValue>> = [
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
];
let Hole: Array<Array<sudokuValue>> = [
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
    [a, a, a, a, a, a, a, a, a],
];
let HolePosition: Array<Array<number>> = [
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
    [b, b, b, b, b, b, b, b, b],
];

export const generateSudoku = (level: Level): sudokuValue[][][] => {
    /*algrithm for generating soduku here*/
    //level对应挖洞个数，1-2-3对应挖洞10,20,30
    for (let i = 0; i < 9; i++) {
        for (let j = 0; j < 9; j++) {
            HolePosition[i][j] = b;
            Hole[i][j] = a;
            Sudoku[i][j] = a;
        }
    }
    while (!fillForm(0, 1));
    digHole((level + 2) * 10);
    getHole();
    return [Hole, Sudoku];
};

export default generateSudoku;

function set(x: number, y: number, val: sudokuValue): boolean {
    if (Sudoku[y][x] !== a) return false;
    let x0: number, y0: number;
    //列冲突
    for (x0 = 0; x0 < 9; x0++) {
        if (Sudoku[y][x0] === val) return false;
    }
    //行冲突
    for (y0 = 0; y0 < 9; y0++) {
        if (Sudoku[y0][x] === val) return false;
    }
    //九宫格内部冲突
    for (y0 = Math.floor(y / 3) * 3; y0 < Math.floor(y / 3) * 3 + 3; y0++) {
        for (x0 = Math.floor(x / 3) * 3; x0 < Math.floor(x / 3) * 3 + 3; x0++) {
            if (Sudoku[y0][x0] === val) return false;
        }
    }
    //没有错误则修改值
    Sudoku[y][x] = val;
    return true;
}
function reset(x: number, y: number) {
    Sudoku[y][x] = a;
}
//产生一个0-9的随机序列
function initXOrd(Xord: Array<number>) {
    let i: number, k: number, temp: number;
    for (i = 0; i < 9; i++) {
        Xord[i] = i;
    }
    for (i = 0; i < 9; i++) {
        //0-9随机数
        k = Math.floor(Math.random() * 9);
        temp = Xord[k];
        Xord[k] = Xord[i];
        Xord[i] = temp;
    }
}
function fillForm(y: number, val: sudokuValue): boolean {
    let Xord: Array<number> = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    let i: number, x: number;
    //随机生成一个序列0-9
    initXOrd(Xord);
    for (i = 0; i < 9; i++) {
        x = Xord[i];
        if (set(x, y, val)) {
            if (y === 8) {
                if (val === 9 || fillForm(0, ((val as number) + 1) as sudokuValue)) return true;
            } else {
                if (fillForm(y + 1, val)) return true;
            }
            reset(x, y);
        }
    }
    return false;
}
//找到挖洞的位置
//修改holeposition
function digHole(holeNum: number) {
    console.log("进入dighole");
    // console.log("%d", holeNum);
    let idx = new Array<number>(81);
    let i: number, k: number, temp: number;
    for (i = 0; i < 81; i++) {
        HolePosition[Math.floor(i / 9)][i % 9] = 0;
        idx[i] = i;
    }

    for (i = 0; i < holeNum; i++) {
        k = Math.floor(Math.random() * 81);
        temp = idx[k];
        idx[k] = idx[i];
        idx[i] = temp;
    }
    for (i = 0; i < holeNum; i++) {
        HolePosition[Math.floor(idx[i] / 9)][idx[i] % 9] = 1;
    }
}



function getHole() {
    let x: number, y: number;
    for (y = 0; y < 9; y++) {
        for (x = 0; x < 9; x++) {
            if (HolePosition[y][x] === 1) {
                Hole[y][x] = a;
            } else {
                Hole[y][x] = Sudoku[y][x];
            }
        }
    }
}
