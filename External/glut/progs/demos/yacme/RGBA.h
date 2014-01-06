
/*
	Initially default system lookup table
	-------------------------------------
*/
static struct
{
	float r, g, b, a;
}
RGBA[256] =
{
	{ 0.000000, 0.000000, 0.000000, 0.000000 },
	{ 1.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 1.000000, 0.000000, 1.000000 },
	{ 1.000000, 1.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 1.000000, 1.000000 },
	{ 1.000000, 0.000000, 1.000000, 1.000000 },
	{ 0.000000, 1.000000, 1.000000, 1.000000 },
	{ 1.000000, 1.000000, 1.000000, 1.000000 },
	{ 0.333333, 0.333333, 0.333333, 1.000000 },
	{ 0.776471, 0.443137, 0.443137, 1.000000 },
	{ 0.443137, 0.776471, 0.443137, 1.000000 },
	{ 0.556863, 0.556863, 0.219608, 1.000000 },
	{ 0.443137, 0.443137, 0.776471, 1.000000 },
	{ 0.556863, 0.219608, 0.556863, 1.000000 },
	{ 0.219608, 0.556863, 0.556863, 1.000000 },
	{ 0.666667, 0.666667, 0.666667, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.039216, 0.039216, 0.039216, 1.000000 },
	{ 0.078431, 0.078431, 0.078431, 1.000000 },
	{ 0.117647, 0.117647, 0.117647, 1.000000 },
	{ 0.156863, 0.156863, 0.156863, 1.000000 },
	{ 0.200000, 0.200000, 0.200000, 1.000000 },
	{ 0.239216, 0.239216, 0.239216, 1.000000 },
	{ 0.278431, 0.278431, 0.278431, 1.000000 },
	{ 0.317647, 0.317647, 0.317647, 1.000000 },
	{ 0.356863, 0.356863, 0.356863, 1.000000 },
	{ 0.400000, 0.400000, 0.400000, 1.000000 },
	{ 0.439216, 0.439216, 0.439216, 1.000000 },
	{ 0.478431, 0.478431, 0.478431, 1.000000 },
	{ 0.517647, 0.517647, 0.517647, 1.000000 },
	{ 0.556863, 0.556863, 0.556863, 1.000000 },
	{ 0.600000, 0.600000, 0.600000, 1.000000 },
	{ 0.639216, 0.639216, 0.639216, 1.000000 },
	{ 0.678431, 0.678431, 0.678431, 1.000000 },
	{ 0.717647, 0.717647, 0.717647, 1.000000 },
	{ 0.756863, 0.756863, 0.756863, 1.000000 },
	{ 0.800000, 0.800000, 0.800000, 1.000000 },
	{ 0.839216, 0.839216, 0.839216, 1.000000 },
	{ 0.878431, 0.878431, 0.878431, 1.000000 },
	{ 0.917647, 0.917647, 0.917647, 1.000000 },
	{ 0.956863, 0.956863, 0.956863, 1.000000 },
	{ 0.000000, 0.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.141176, 0.000000, 1.000000 },
	{ 0.000000, 0.282353, 0.000000, 1.000000 },
	{ 0.000000, 0.427451, 0.000000, 1.000000 },
	{ 0.000000, 0.568627, 0.000000, 1.000000 },
	{ 0.000000, 0.713726, 0.000000, 1.000000 },
	{ 0.000000, 0.854902, 0.000000, 1.000000 },
	{ 0.000000, 1.000000, 0.000000, 1.000000 },
	{ 0.247059, 0.000000, 0.000000, 1.000000 },
	{ 0.247059, 0.141176, 0.000000, 1.000000 },
	{ 0.247059, 0.282353, 0.000000, 1.000000 },
	{ 0.247059, 0.427451, 0.000000, 1.000000 },
	{ 0.247059, 0.568627, 0.000000, 1.000000 },
	{ 0.247059, 0.713726, 0.000000, 1.000000 },
	{ 0.247059, 0.854902, 0.000000, 1.000000 },
	{ 0.247059, 1.000000, 0.000000, 1.000000 },
	{ 0.498039, 0.000000, 0.000000, 1.000000 },
	{ 0.498039, 0.141176, 0.000000, 1.000000 },
	{ 0.498039, 0.282353, 0.000000, 1.000000 },
	{ 0.498039, 0.427451, 0.000000, 1.000000 },
	{ 0.498039, 0.568627, 0.000000, 1.000000 },
	{ 0.498039, 0.713726, 0.000000, 1.000000 },
	{ 0.498039, 0.854902, 0.000000, 1.000000 },
	{ 0.498039, 1.000000, 0.000000, 1.000000 },
	{ 0.749020, 0.000000, 0.000000, 1.000000 },
	{ 0.749020, 0.141176, 0.000000, 1.000000 },
	{ 0.749020, 0.282353, 0.000000, 1.000000 },
	{ 0.749020, 0.427451, 0.000000, 1.000000 },
	{ 0.749020, 0.568627, 0.000000, 1.000000 },
	{ 0.749020, 0.713726, 0.000000, 1.000000 },
	{ 0.749020, 0.854902, 0.000000, 1.000000 },
	{ 0.749020, 1.000000, 0.000000, 1.000000 },
	{ 1.000000, 0.000000, 0.000000, 1.000000 },
	{ 1.000000, 0.141176, 0.000000, 1.000000 },
	{ 1.000000, 0.282353, 0.000000, 1.000000 },
	{ 1.000000, 0.427451, 0.000000, 1.000000 },
	{ 1.000000, 0.568627, 0.000000, 1.000000 },
	{ 1.000000, 0.713726, 0.000000, 1.000000 },
	{ 1.000000, 0.854902, 0.000000, 1.000000 },
	{ 1.000000, 1.000000, 0.000000, 1.000000 },
	{ 0.000000, 0.000000, 0.247059, 1.000000 },
	{ 0.000000, 0.141176, 0.247059, 1.000000 },
	{ 0.000000, 0.282353, 0.247059, 1.000000 },
	{ 0.000000, 0.427451, 0.247059, 1.000000 },
	{ 0.000000, 0.568627, 0.247059, 1.000000 },
	{ 0.000000, 0.713726, 0.247059, 1.000000 },
	{ 0.000000, 0.854902, 0.247059, 1.000000 },
	{ 0.000000, 1.000000, 0.247059, 1.000000 },
	{ 0.247059, 0.000000, 0.247059, 1.000000 },
	{ 0.247059, 0.141176, 0.247059, 1.000000 },
	{ 0.247059, 0.282353, 0.247059, 1.000000 },
	{ 0.247059, 0.427451, 0.247059, 1.000000 },
	{ 0.247059, 0.568627, 0.247059, 1.000000 },
	{ 0.247059, 0.713726, 0.247059, 1.000000 },
	{ 0.247059, 0.854902, 0.247059, 1.000000 },
	{ 0.247059, 1.000000, 0.247059, 1.000000 },
	{ 0.498039, 0.000000, 0.247059, 1.000000 },
	{ 0.498039, 0.141176, 0.247059, 1.000000 },
	{ 0.498039, 0.282353, 0.247059, 1.000000 },
	{ 0.498039, 0.427451, 0.247059, 1.000000 },
	{ 0.498039, 0.568627, 0.247059, 1.000000 },
	{ 0.498039, 0.713726, 0.247059, 1.000000 },
	{ 0.498039, 0.854902, 0.247059, 1.000000 },
	{ 0.498039, 1.000000, 0.247059, 1.000000 },
	{ 0.749020, 0.000000, 0.247059, 1.000000 },
	{ 0.749020, 0.141176, 0.247059, 1.000000 },
	{ 0.749020, 0.282353, 0.247059, 1.000000 },
	{ 0.749020, 0.427451, 0.247059, 1.000000 },
	{ 0.749020, 0.568627, 0.247059, 1.000000 },
	{ 0.749020, 0.713726, 0.247059, 1.000000 },
	{ 0.749020, 0.854902, 0.247059, 1.000000 },
	{ 0.749020, 1.000000, 0.247059, 1.000000 },
	{ 1.000000, 0.000000, 0.247059, 1.000000 },
	{ 1.000000, 0.141176, 0.247059, 1.000000 },
	{ 1.000000, 0.282353, 0.247059, 1.000000 },
	{ 1.000000, 0.427451, 0.247059, 1.000000 },
	{ 1.000000, 0.568627, 0.247059, 1.000000 },
	{ 1.000000, 0.713726, 0.247059, 1.000000 },
	{ 1.000000, 0.854902, 0.247059, 1.000000 },
	{ 1.000000, 1.000000, 0.247059, 1.000000 },
	{ 0.000000, 0.000000, 0.498039, 1.000000 },
	{ 0.000000, 0.141176, 0.498039, 1.000000 },
	{ 0.000000, 0.282353, 0.498039, 1.000000 },
	{ 0.000000, 0.427451, 0.498039, 1.000000 },
	{ 0.000000, 0.568627, 0.498039, 1.000000 },
	{ 0.000000, 0.713726, 0.498039, 1.000000 },
	{ 0.000000, 0.854902, 0.498039, 1.000000 },
	{ 0.000000, 1.000000, 0.498039, 1.000000 },
	{ 0.247059, 0.000000, 0.498039, 1.000000 },
	{ 0.247059, 0.141176, 0.498039, 1.000000 },
	{ 0.247059, 0.282353, 0.498039, 1.000000 },
	{ 0.247059, 0.427451, 0.498039, 1.000000 },
	{ 0.247059, 0.568627, 0.498039, 1.000000 },
	{ 0.247059, 0.713726, 0.498039, 1.000000 },
	{ 0.247059, 0.854902, 0.498039, 1.000000 },
	{ 0.247059, 1.000000, 0.498039, 1.000000 },
	{ 0.498039, 0.000000, 0.498039, 1.000000 },
	{ 0.498039, 0.141176, 0.498039, 1.000000 },
	{ 0.498039, 0.282353, 0.498039, 1.000000 },
	{ 0.498039, 0.427451, 0.498039, 1.000000 },
	{ 0.498039, 0.568627, 0.498039, 1.000000 },
	{ 0.498039, 0.713726, 0.498039, 1.000000 },
	{ 0.498039, 0.854902, 0.498039, 1.000000 },
	{ 0.498039, 1.000000, 0.498039, 1.000000 },
	{ 0.749020, 0.000000, 0.498039, 1.000000 },
	{ 0.749020, 0.141176, 0.498039, 1.000000 },
	{ 0.749020, 0.282353, 0.498039, 1.000000 },
	{ 0.749020, 0.427451, 0.498039, 1.000000 },
	{ 0.749020, 0.568627, 0.498039, 1.000000 },
	{ 0.749020, 0.713726, 0.498039, 1.000000 },
	{ 0.749020, 0.854902, 0.498039, 1.000000 },
	{ 0.749020, 1.000000, 0.498039, 1.000000 },
	{ 1.000000, 0.000000, 0.498039, 1.000000 },
	{ 1.000000, 0.141176, 0.498039, 1.000000 },
	{ 1.000000, 0.282353, 0.498039, 1.000000 },
	{ 1.000000, 0.427451, 0.498039, 1.000000 },
	{ 1.000000, 0.568627, 0.498039, 1.000000 },
	{ 1.000000, 0.713726, 0.498039, 1.000000 },
	{ 1.000000, 0.854902, 0.498039, 1.000000 },
	{ 1.000000, 1.000000, 0.498039, 1.000000 },
	{ 0.000000, 0.000000, 0.749020, 1.000000 },
	{ 0.000000, 0.141176, 0.749020, 1.000000 },
	{ 0.000000, 0.282353, 0.749020, 1.000000 },
	{ 0.000000, 0.427451, 0.749020, 1.000000 },
	{ 0.000000, 0.568627, 0.749020, 1.000000 },
	{ 0.000000, 0.713726, 0.749020, 1.000000 },
	{ 0.000000, 0.854902, 0.749020, 1.000000 },
	{ 0.000000, 1.000000, 0.749020, 1.000000 },
	{ 0.247059, 0.000000, 0.749020, 1.000000 },
	{ 0.247059, 0.141176, 0.749020, 1.000000 },
	{ 0.247059, 0.282353, 0.749020, 1.000000 },
	{ 0.247059, 0.427451, 0.749020, 1.000000 },
	{ 0.247059, 0.568627, 0.749020, 1.000000 },
	{ 0.247059, 0.713726, 0.749020, 1.000000 },
	{ 0.247059, 0.854902, 0.749020, 1.000000 },
	{ 0.247059, 1.000000, 0.749020, 1.000000 },
	{ 0.498039, 0.000000, 0.749020, 1.000000 },
	{ 0.498039, 0.141176, 0.749020, 1.000000 },
	{ 0.498039, 0.282353, 0.749020, 1.000000 },
	{ 0.498039, 0.427451, 0.749020, 1.000000 },
	{ 0.498039, 0.568627, 0.749020, 1.000000 },
	{ 0.498039, 0.713726, 0.749020, 1.000000 },
	{ 0.498039, 0.854902, 0.749020, 1.000000 },
	{ 0.498039, 1.000000, 0.749020, 1.000000 },
	{ 0.749020, 0.000000, 0.749020, 1.000000 },
	{ 0.749020, 0.141176, 0.749020, 1.000000 },
	{ 0.749020, 0.282353, 0.749020, 1.000000 },
	{ 0.749020, 0.427451, 0.749020, 1.000000 },
	{ 0.749020, 0.568627, 0.749020, 1.000000 },
	{ 0.749020, 0.713726, 0.749020, 1.000000 },
	{ 0.749020, 0.854902, 0.749020, 1.000000 },
	{ 0.749020, 1.000000, 0.749020, 1.000000 },
	{ 1.000000, 0.000000, 0.749020, 1.000000 },
	{ 1.000000, 0.141176, 0.749020, 1.000000 },
	{ 1.000000, 0.282353, 0.749020, 1.000000 },
	{ 1.000000, 0.427451, 0.749020, 1.000000 },
	{ 1.000000, 0.568627, 0.749020, 1.000000 },
	{ 1.000000, 0.713726, 0.749020, 1.000000 },
	{ 1.000000, 0.854902, 0.749020, 1.000000 },
	{ 1.000000, 1.000000, 0.749020, 1.000000 }, 
	{ 0.000000, 0.000000, 1.000000, 1.000000 },
	{ 0.000000, 0.141176, 1.000000, 1.000000 },
	{ 0.000000, 0.282353, 1.000000, 1.000000 },
	{ 0.000000, 0.427451, 1.000000, 1.000000 },
	{ 0.000000, 0.568627, 1.000000, 1.000000 },
	{ 0.000000, 0.713726, 1.000000, 1.000000 },
	{ 0.000000, 0.854902, 1.000000, 1.000000 },
	{ 0.000000, 1.000000, 1.000000, 1.000000 },
	{ 0.247059, 0.000000, 1.000000, 1.000000 },
	{ 0.247059, 0.141176, 1.000000, 1.000000 },
	{ 0.247059, 0.282353, 1.000000, 1.000000 },
	{ 0.247059, 0.427451, 1.000000, 1.000000 },
	{ 0.247059, 0.568627, 1.000000, 1.000000 },
	{ 0.247059, 0.713726, 1.000000, 1.000000 },
	{ 0.247059, 0.854902, 1.000000, 1.000000 },
	{ 0.247059, 1.000000, 1.000000, 1.000000 },
	{ 0.498039, 0.000000, 1.000000, 1.000000 },
	{ 0.498039, 0.141176, 1.000000, 1.000000 },
	{ 0.498039, 0.282353, 1.000000, 1.000000 },
	{ 0.498039, 0.427451, 1.000000, 1.000000 },
	{ 0.498039, 0.568627, 1.000000, 1.000000 },
	{ 0.498039, 0.713726, 1.000000, 1.000000 },
	{ 0.498039, 0.854902, 1.000000, 1.000000 },
	{ 0.498039, 1.000000, 1.000000, 1.000000 },
	{ 0.749020, 0.000000, 1.000000, 1.000000 },
	{ 0.749020, 0.141176, 1.000000, 1.000000 },
	{ 0.749020, 0.282353, 1.000000, 1.000000 },
	{ 0.749020, 0.427451, 1.000000, 1.000000 },
	{ 0.749020, 0.568627, 1.000000, 1.000000 },
	{ 0.749020, 0.713726, 1.000000, 1.000000 },
	{ 0.749020, 0.854902, 1.000000, 1.000000 },
	{ 0.749020, 1.000000, 1.000000, 1.000000 },
	{ 1.000000, 0.000000, 1.000000, 1.000000 },
	{ 1.000000, 0.141176, 1.000000, 1.000000 },
	{ 1.000000, 0.282353, 1.000000, 1.000000 },
	{ 1.000000, 0.427451, 1.000000, 1.000000 },
	{ 1.000000, 0.568627, 1.000000, 1.000000 },
	{ 1.000000, 0.713726, 1.000000, 1.000000 },
	{ 1.000000, 0.854902, 1.000000, 1.000000 },
	{ 1.000000, 1.000000, 1.000000, 1.000000 }
};