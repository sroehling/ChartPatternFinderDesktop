#ifndef VOLUMEYAXISSCALEDRAW_H
#define VOLUMEYAXISSCALEDRAW_H

#include <qwt_scale_draw.h>

class VolumeYAxisScaleDraw : public QwtScaleDraw
{
 public:
    VolumeYAxisScaleDraw();

    virtual QwtText label( double labelVal ) const;
};

#endif // VOLUMEYAXISSCALEDRAW_H
