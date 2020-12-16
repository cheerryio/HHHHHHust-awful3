import React, { FC, memo, SVGProps } from 'react';
import multiavatar from '@multiavatar/multiavatar';
export const AvatarSvg: FC<{
    SvgProp?: SVGProps<SVGSVGElement>;
    GProp?: SVGProps<SVGGElement>;
    AvatarId?: string;
}> = memo(({ SvgProp = {}, GProp = {}, AvatarId = 'bind avatar' }) => {
    let pathCode: string = multiavatar(AvatarId).match(/<svg[^>]*>(.*)<\/svg>/)[1];
    return (
        <svg {...SvgProp} viewBox={'0 0 231 231'}>
            <g dangerouslySetInnerHTML={{ __html: pathCode }} {...GProp}></g>
        </svg>
    );
});

export default AvatarSvg;
