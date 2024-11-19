<!DOCTYPE html>
<html lang="es">

<head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <!-- Link a BOOTSTRAP -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous" />
    <!-- Link a CSS -->
    <link rel="stylesheet" href="./style.css" />
    <!-- Link a Javascrypt -->

    <!-- Logo Pagina -->
    <link rel="shortcut icon" href="img/logo_tacher.png">
    <!-- Link a Boxicons -->
    <link href="https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css" rel="stylesheet" />
    <!-- Fuente -->
    <link rel="preconnect" href="https://fonts.googleapis.com" />
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
    <link href="https://fonts.googleapis.com/css2?family=Poppins:ital,wght@0,500;0,600;1,500;1,600&display=swap" rel="stylesheet" />
    <title>Tacher2</title>
</head>

<body>
    <header>
        <div class="container">
            <header class="d-flex flex-wrap align-items-center justify-content-center justify-content-md-between py-3">
                <div class="col-md-3 mb-2 mb-md-0">
                    <a class="navbar-brand" href="index.html"> <img class="logo-tacher" src="img/logo_tacher_white.png" alt="" /><!-- hay que poner el logo blanco --> </a>
                </div>
                <ul class="nav col-12 col-md-auto mb-2 justify-content-center mb-md-0">
                    <li>
                        <div class="elem-nav-i-f-sn">
                            <a href="index.html" class="nav-link px-2 header_text">Inicio</a>
                        </div>
                    </li>
                    <li>
                        <div class="elem-nav-i-f-sn">
                            <a href="index.html#sobre-nosotros" class="nav-link px-2 header_text">Sobre Nosotros</a>
                        </div>
                    </li>
                    <li>
                        <div class="elem-nav-i-f-sn">
                            <a href="index.html#contacto" class="nav-link px-2 header_text">Contacto</a>
                        </div>
                    </li>
                </ul>

                <div class="col-md-3 text-end">
                    <a href="login.php"><button type="button" href="#" class="btn btn-outline-primary me-2 header_buttoms">Iniciar Sesion</button></a>
                </div>
            </header>
        </div>
    </header>

    <div class="container  centrar2">
        <div class="row justify-content-center">
            <div class="col-md-4">
                    <!-- <?php if (isset($_GET["rgt"]) and $_GET["rgt"] == "1") { ?>
                        <script>
                            alert("El usuario se registró correctamente. A continuación podrá iniciar sesión...");
                            location.href = 'login.php';
                        </script>
                    <?php } else if (isset($_GET["rgt"]) and $_GET["rgt"] == "0") { ?>
                        <script>
                            alert("El usuario no se pudo registrar correctamente.");
                        </script>
                    <?php } ?> -->

                <form id="form" method="POST" action="registroCTRL.php">
                    <div class="mb-3">
                        <label for="int_dni_usuario" class="form-label">Número de DNI</label>
                        <input type="text" class="form-control" id="int_dni_usuario" name="int_dni_usuario" placeholder="Introduzca su DNI" pattern="[0-9]{8}" title="Debe ingresar su DNI" required="" minlength="8" maxlength="8" />
                    </div>

                    <div class="mb-3">
                        <label for="txt_nombre_usuario" class="form-label">Usuario</label>
                        <input type="text" class="form-control" id="txt_nombre_usuario" name="txt_nombre_usuario" placeholder="Introduza su nombre" required="" minlength="4" maxlength="12" />
                    </div>

                    <button type="submit" class="btn btn-primary w-100 text-uppercase fw-bold">Registrarse</button>
                </form>
            </div>
        </div>
    </div>
    <footer>
        <div class="fluid-container black ">
            <div class="row g-0 d-flex">
                <div class="col-4 offset-1 py-2 pb-2 d-none d-md-block">
                    <img src="img/logo_tacher.png" alt="Logo" height="40vh" />
                </div>
                <div class="col-10 offset-1 py-2 pb-2 d-md-none text-center mt-3">
                    <img src="img/logo_tacher.png" alt="Logo" height="40vh" class="mb-3" />
                </div>
                <div class="col-auto ms-auto my-auto d-none d-md-block">
                    <div class="text-center">
                        <a href="https://www.facebook.com/escuelatecnica36" class="text-decoration-none" target="_blank">
                            <i class="bx bxl-facebook-circle facebooket footer-iconos"></i>
                        </a>
                        <a href="https://mail.google.com/" class="text-decoration-none">
                            <i class="bx bxl-gmail gmail footer-iconos"></i>
                        </a>
                        <a href="https://www.google.com/maps/place/Escuela+T%C3%A9cnica+36+D.E.+15+-+Alte.+Brown/@-34.5607942,-58.4942549,16.81z/data=!4m6!3m5!1s0x95bcb6f1ce409681:0xed70920e983fcb6a!8m2!3d-34.5606214!4d-58.4939421!16s%2Fg%2F11c55rrf0l?entry=ttu" class="text-decoration-none">
                            <i class="bx bxs-map mapa footer-iconos"></i>
                        </a>
                    </div>
                </div>
                <div class="col-1"></div>
                <div class="col-10 offset-1 border-bottom grey-border"></div>
                <div class="col-10 offset-1 border-bottom grey-border text-center white">
                    <div class="col-12 d-md-none">
                        <div class="text-center mb-1">
                            <a href="https://www.facebook.com/escuelatecnica36" target="_blank" class="ms-auto me-1 text-decoration-none">
                                <i class="bx bxl-facebook-circle facebooket"></i>
                            </a>
                            <a href="https://mail.google.com/" class="text-decoration-none">
                                <i class="bx bxl-gmail gmail"></i>
                            </a>
                            <a href="https://www.google.com/maps/place/Escuela+T%C3%A9cnica+36+D.E.+15+-+Alte.+Brown/@-34.5607942,-58.4942549,16.81z/data=!4m6!3m5!1s0x95bcb6f1ce409681:0xed70920e983fcb6a!8m2!3d-34.5606214!4d-58.4939421!16s%2Fg%2F11c55rrf0l?entry=ttu" class="text-decoration-none">
                                <i class="bx bxs-map mapa"></i>
                            </a>
                        </div>
                    </div>
                        <a href="tel:+5491165742379" class="text-black footer-palabras">+54 9 11 6574-2379</a>
                        <a href="mailto:tacher2consultas@gmail.com" class="text-black footer-palabras">
                            <h5 class="mailtext">tacher2consultas@gmail.com</h5>
                        </a>
                </div>
            </div>
            <div class="col-10 offset-1 py-2 pb-0 text-center small">Tacher © 2023 | All rights reserved.</div>
        </div>
    </footer>

    <!-- SCRIPTS -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js" integrity="sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM" crossorigin="anonymous"></script>
    <script src="https://unpkg.com/boxicons@2.1.4/dist/boxicons.js"></script>
</body>

</html>