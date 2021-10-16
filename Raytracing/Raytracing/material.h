#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
public:
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered) const = 0;
	
};

class lambertian : public material {
public:
	lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		//catch degenerate scatter direction of near zero
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
public:
	color albedo;
};

class metal : public material {
public:
	metal(const color& a, double f) : albedo(a),fuzz(f<1 ? f:1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz; //this is smooth vs rough metal
};

class dielectric : public material {
public:
	dielecctric(double index_of_refection) : ir(index_of_refraction) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color attentuation, ray& scattered
	) const override {
		attentuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());
		vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, refracted);
		return true;
	}
public:
	double ir; //index of refraction
};
#endif