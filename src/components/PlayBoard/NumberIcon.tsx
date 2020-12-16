import React, { FC, memo } from 'react';

import { sudokuValue } from '../../types';
import useStyles from '../../styles/playBoard';
import classNames from 'classnames';
import { NumberSvg } from '../SmallComponents/NumberSvg';

function comparePlayBoardBlock(prevProps: any, nextProps: any) {
    if (prevProps.unchangeable === true) return true;
    return (
        prevProps.num === nextProps.num &&
        prevProps.showUnchangeable === nextProps.showUnchangeable &&
        prevProps.className === nextProps.className
    );
}

const NumberIcon: FC<{
    num: sudokuValue;
    initNum: sudokuValue;
    showUnchangeable: boolean;
    className: any;
}> = memo(({ num, initNum, showUnchangeable, className }) => {
    const classes = useStyles();
    if (initNum !== null)
        return (
            <NumberSvg
                num={initNum as number}
                SvgProp={{
                    className: classNames(className, {
                        [classes.unchangeableBlock]: showUnchangeable,
                    }),
                }}></NumberSvg>
        );
    else {
        if (num === null) {
            return (
                <svg className={classNames(className, {})}>
                    <image width='100%' height='100%' xlinkHref='' />
                </svg>
            );
        } else
            return (
                <NumberSvg
                    num={num as number}
                    SvgProp={{
                        className: classNames(className, {}),
                    }}></NumberSvg>
            );
    }
}, comparePlayBoardBlock);

export default NumberIcon;
