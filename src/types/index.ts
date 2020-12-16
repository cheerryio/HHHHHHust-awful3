export type sudokuValue = null | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9; /** null-格子为空，1-9代表格子值为1-9*/
/** null-此格不与任何格冲突 1-9代表此格是1-9数字的冲突*/
export type conflictValue = null | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9;
export type PlaceValue = -1 | null | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9;
export type Level = 0 | 1 | 2 | 3;
export type Gender = 0 | 1 | 2;

export interface Point {
    x: number;
    y: number;
    value: sudokuValue;
}

export interface PlayHistory {
    x: number;
    y: number;
    from: sudokuValue;
    to: sudokuValue;
}

export interface Message {
    content: string;
    isSelf: boolean;
    name: string;
    time: number;
    isImage: boolean;
    avatar: string;
}

export interface Userinfo {
    _id: string;
    weChatID: string;
    username: string;
    password?: string;
    joinTime: string; // e.g. 2018A || 2018S (A: AUTUMN, S: SPRING, C: CAMP)
    isCaptain: boolean;
    isAdmin: boolean;
    phone: string;
    mail: string;
    gender: Gender;
    avatar: string;
}
