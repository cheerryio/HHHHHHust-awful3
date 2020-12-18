import React, { FC, memo, SVGProps } from 'react';
import { numberIcons } from '../../consts/elements';
export const NumberSvg: FC<{
    num: number;
    SvgProp?: SVGProps<SVGSVGElement>;
    GProp?: SVGProps<SVGGElement>;
}> = memo(({ num, SvgProp = {}, GProp = {} }) => {
    if (num !== -1) {
        return (
            <svg viewBox='0 0 2560 2560' {...SvgProp}>
                <g transform='rotate(180) scale(-1,1)' style={{ transformOrigin: 'center' }} {...GProp}>
                    {numberIcons.get(num)}
                </g>
            </svg>
        );
    } else {
        return (
            <svg viewBox='0 0 24 24' {...SvgProp}>
                <g {...GProp}>{numberIcons.get(num)}</g>
            </svg>
        );
    }
});

export default NumberSvg;
