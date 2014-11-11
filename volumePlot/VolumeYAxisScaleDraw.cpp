#include "VolumeYAxisScaleDraw.h"

VolumeYAxisScaleDraw::VolumeYAxisScaleDraw()
{
}


QwtText VolumeYAxisScaleDraw::label( double labelVal ) const
{
    // The %g in the sprintf will remove trailing zeroes as appropriate.
    QString formattedVolume;
    if(labelVal >= 1000000.0)
     {
        double volumeInMillions = labelVal/1000000.0;
        formattedVolume = QString().sprintf("%gM",volumeInMillions);
    }
    else if(labelVal <= 0.0)
    {
        formattedVolume = QString("0");
    }
    else
    {
        double volumeInThousands = labelVal / 1000.0;
        formattedVolume = QString().sprintf("%gK",volumeInThousands);
    }

    return QwtText(formattedVolume);
}
