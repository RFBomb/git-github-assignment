//
// Robert Brenckman
// ITE215 - Week 7
// Console Summer
//

use std::io::{self, Write};

fn main()
{
    loop
    {
        let x = get_int("Enter the first integer to sum : ");
        let y = get_int("Enter the second integer : ");
        println!("{} + {} = {}",x,y,x+y);
        println!();
    }
}

fn get_int(prompt: &str) -> i32 {
    loop {
        print!("{}", prompt);
        io::stdout().flush().unwrap();

        let mut input = String::new();
        if io::stdin().read_line(&mut input).is_err() {
            println!("Error reading input. Try again.");
            continue;
        }

        match input.trim().parse::<i32>() {
            Ok(value) => return value,
            Err(_) => {
                println!("Please enter a valid integer.");
                continue;
            }
        }
    }
}