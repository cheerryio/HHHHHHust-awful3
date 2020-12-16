import React, { FC, memo, SVGProps } from 'react';
import { loadIcon } from '../../consts/elements';
export const LoadSvg: FC<{
    SvgProp?: SVGProps<SVGSVGElement>;
    GProp?: SVGProps<SVGGElement>;
}> = memo(({ SvgProp = {}, GProp = {} }) => (
    <svg viewBox='0 0 24 24' {...SvgProp}>
        <g {...GProp}>{loadIcon}</g>
    </svg>
));

export default LoadSvg;
