export const digitsKeyMap = {
    1: ['1'],
    2: ['2'],
    3: ['3'],
    4: ['4'],
    5: ['5'],
    6: ['6'],
    7: ['7'],
    8: ['8'],
    9: ['9'],
};

export function createDigitsHandlers(handler, ...args) {
    let handlers = {};
    Object.keys(digitsKeyMap).forEach((key, index) => {
        handlers[key] = function () {
            handler(index + 1, args);
        };
    });
    return handlers;
}
