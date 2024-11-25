-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Servidor: localhost:3306
-- Tiempo de generación: 22-08-2024 a las 03:23:17
-- Versión del servidor: 10.5.20-MariaDB
-- Versión de PHP: 7.3.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

--
-- Base de datos: id22134803_db_test
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla registro_deposito
--

CREATE TABLE registro_deposito (
  id int(11) NOT NULL,
  dni_usuario bigint(20) NOT NULL,
  code_producto bigint(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla registro_deposito
--
ALTER TABLE registro_deposito
  ADD PRIMARY KEY (id),
  ADD KEY dni_usuario (dni_usuario),
  ADD KEY code_producto (code_producto);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla registro_deposito
--
ALTER TABLE registro_deposito
  MODIFY id int(11) NOT NULL AUTO_INCREMENT;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla registro_deposito
--
ALTER TABLE registro_deposito
  ADD CONSTRAINT registro_deposito_ibfk_1 FOREIGN KEY (dni_usuario) REFERENCES usuarios (dni),
  ADD CONSTRAINT registro_deposito_ibfk_2 FOREIGN KEY (code_producto) REFERENCES productos (code_barras);
COMMIT;
