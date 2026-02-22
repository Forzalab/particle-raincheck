#include "Bifrost.h"

Bifrost::Bifrost(const Param &api_key, const Param &username)
	: api_key(api_key), username(username), bridge(username, api_key) {}

// Getters OUTSIDE Bifrost
wXY Bifrost::getWorldBound(const WorldSnapshot &ws) {
	Wc x = ws.get_rows();
	Wc y = ws.get_cols();
	return {x, y};
}

// x y to .xy
void pState::setXY(const ParticleCoordsValue &x, const ParticleCoordsValue &y) {
	pXY &xy = this->xy;
	xy.row = x;
	xy.col = y;
}

// r g b to .rgb
void pState::setRGB(const ParticleColorValue &r, const ParticleColorValue &g,
			const ParticleColorValue &b) {
	pRGB &rgb = this->rgb;
	rgb.red = r;
	rgb.green = g;
	rgb.blue = b;
}

// Convention for op func:
// - operator<< means insert
// - operator<<= means "assign partially/narrowing" PLUS
//   pointer resolve. It involves type/data loss
//   (cuz it looks like a funnel)
// - operator<= means total conversion (cuz it looks like
//   an arrow)

// Chainable ops would return NOT bool type!

// particle <<= p_ptr
pState &operator<<=(pState &particle, const P_ptr &p) {
        const Pc x = p->get_row(), y = p->get_col();
	const Color r = p->get_r(), g = p->get_g(), b = p->get_b();
	
	particle.setXY(x,y);
	particle.setRGB(r,g,b);
	
	return particle;
}

// of stage (1), particle.rgb into BColor
void operator<=(BColor &bc, const pRGB &rgb) {
	bc.setRed(rgb.red);
	bc.setGreen(rgb.green);
	bc.setBlue(rgb.blue);
}

// two stages:
// (1) resolve color -> color and xy -> x y.
// (2) feed into BCG
void operator<<(BColorGrid &bcg, const pState &particle) {
	BColor bc;
	const pXY pxy = particle.xy;

	bc <= particle.rgb;

	// For duplicate particles: last dupl par. is counted
	bcg.set(pxy.row, pxy.col, bc);
}

// BCG <<= ws
// Map color of a world snapshot.
void operator<<=(BColorGrid &bcg, WorldSnapshot &ws) {
	Ps ps = ws.getParticles();
	wXY bounds = Bifrost::getWorldBound(ws);
	pState p{};

	for (P_ptr p_ptr : ps)
		bcg << (p <<= p_ptr);
}

// url <= BCG
void operator<=(VisualizerURL &url, const BColorGrid &bcg) {

}

// Out-of-the-box BRIDGES visualizer
void operator<<=(VisualizerURL &url, WorldSnapshot &ws) {
}

