import React, { FC, memo, SVGProps } from 'react';
import { numberIcons } from '../../consts/elements';
import Clear from '@material-ui/icons/Clear';
import { SvgIconProps } from '@material-ui/core/SvgIcon';
export const NumberSvg: FC<{
    num: number;
    SvgProp?: SVGProps<SVGSVGElement>;
    GProp?: SVGProps<SVGGElement>;
    ClearProp?: SvgIconProps;
}> = memo(({ num, SvgProp = {}, GProp = {}, ClearProp = {} }) => {
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
            <svg {...SvgProp}>
                <Clear width='100%' height='100%' {...ClearProp} />
            </svg>
        );
    }
});

export default NumberSvg;
